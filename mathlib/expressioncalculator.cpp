//========= Copyright c 1996-2008, Valve Corporation, All rights reserved. ============//

#include "tier0/platform.h"
#include "tier0/dbg.h"

#include "mathlib/mathlib.h"
#include "mathlib/noise.h"
#include "mathlib/vector.h"

#include "mathlib/expressioncalculator.h"

#include <ctype.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


inline void CExpressionCalculator::AddOperator( tOperator op )
{
	COMPILE_TIME_ASSERT( sizeof( tOperator ) == 1 );

	m_instrs.AddToTail( ( uint8 )op );
}

inline void CExpressionCalculator::AddFunc( tFunction func )
{
#if EXPRESSIONCALCULATOR_USE_EXPRESSIVE_PARSE
	COMPILE_TIME_ASSERT( sizeof( tFunction ) == 4 );
	int pos = m_instrs.AddMultipleToTail( 4 );
	memcpy( &m_instrs[pos], &func, 4 );
#else
	COMPILE_TIME_ASSERT( sizeof( tFunction ) == 1 );
	m_instrs.AddToTail( ( uint8 )func );
#endif
}

inline void CExpressionCalculator::AddVar( int varIndex )
{
	// can't handle more than 256 vars
	Assert( varIndex < 256 );
	m_instrs.AddToTail( ( uint8 )varIndex );
}

inline void CExpressionCalculator::AddLiteral( float lit )
{
	int pos = m_instrs.AddMultipleToTail( sizeof(float) );
	memcpy( &m_instrs[pos], &lit, sizeof(float) );
}

inline CExpressionCalculator::tOperator CExpressionCalculator::ReadOperator( uint8* &instr )
{
	tOperator result = ( tOperator )*instr;
	instr++;

	return result;
}

inline CExpressionCalculator::tFunction CExpressionCalculator::ReadFunc( uint8* &instr )
{
	tFunction result;

#if EXPRESSIONCALCULATOR_USE_EXPRESSIVE_PARSE
	COMPILE_TIME_ASSERT( sizeof( tFunction ) == 4 );
	memcpy( &result, instr, 4 );
	instr += 4;
#else
	COMPILE_TIME_ASSERT( sizeof( tFunction ) == 1 );
	result = ( tFunction )*instr;
	instr++;
#endif

	return result;
}

inline int CExpressionCalculator::ReadVar( uint8* &instr )
{
	int result = (int)*instr;
	instr++;

	return result;
}

inline float CExpressionCalculator::ReadLiteral( uint8* &instr )
{
	float result;

	memcpy( &result, instr, sizeof( float ) );
	instr += sizeof( float );

	return result;
}

//-----------------------------------------------------------------------------
// Parsing helper methods
//-----------------------------------------------------------------------------
bool ParseLiteral( const char *&expr, float &value )
{
	const char *startExpr = expr;
	value = ( float )strtod( startExpr, const_cast< char** >( &expr ) );
	return ( startExpr != expr );
}

bool ParseString( const char *&expr, const char *str )
{
	const char *startExpr = expr;
	while ( ( *expr == ' ' ) || ( *expr == '\t' ) )
		expr++; // skip whitespace

	expr = StringAfterPrefix( expr, str );
	if ( expr )
		return true;

	expr = startExpr;
	return false;
}

bool ParseStringList( const char *&expr, const char **pOps, int &nOp )
{
	while ( nOp-- )
	{
		if ( ParseString( expr, pOps[ nOp ] ) )
			return true;
	}
	return false;
}

bool ParseStringList( const char *&expr, const CUtlVector< CUtlString > &strings, int &nOp )
{
	while ( nOp-- )
	{
		if ( ParseString( expr, strings[ nOp ] ) )
			return true;
	}
	return false;
}

int FindString( const CUtlVector< CUtlString > &strings, const char *str )
{
	uint sn = strings.Count();
	for ( uint si = 0; si < sn; ++si )
	{
		if ( !Q_strcmp( str, strings[ si ] ) )
			return si;
	}
	return -1;
}

class ParseState_t
{
public:
	ParseState_t( const CUtlVector<uint8> &instrs, const char *expr )
		: m_numinstrs( instrs.Count() ), m_startingExpr( expr ) {}
	void Reset( CUtlVector<uint8> &instrs, const char *&expr )
	{
		Assert( m_numinstrs <= instrs.Count() );
		instrs.RemoveMultipleFromTail( instrs.Count() - m_numinstrs );
		expr = m_startingExpr;
	}

private:
	int m_numinstrs;
	const char* m_startingExpr;
};




void CExpressionCalculator::SetVariable( int nVariableIndex, float value )
{
	ValidateExpr();
	m_varValues[ nVariableIndex ] = value;
}

void CExpressionCalculator::SetVariable( const char *var, float value )
{
	ValidateExpr();
	int vi = FindString( m_varNames, var );
	if ( vi >= 0 )
		m_varValues[ vi ] = value;
}

void CExpressionCalculator::ModifyVariable( const char *var, float value )
{
	ValidateExpr();
	int vi = FindString( m_varNames, var );
	if ( vi >= 0 )
		m_varValues[ vi ] += value;
}

int CExpressionCalculator::FindVariableIndex( const char *var )
{
	ValidateExpr();
	return FindString( m_varNames, var );
}

bool CExpressionCalculator::Evaluate( float &value )
{
	// Make sure we have parsed our expression
	ValidateExpr();

	m_stack.Clear();
	if ( !m_instrs.IsEmpty() )
	{
		value = Eval( &m_stack, m_instrs.Base(), m_instrs.Base() + m_instrs.Count(), m_varValues.Base(), m_varValues.Count() );
		return true;
	}

	value = 0.0f;
	return false;
}


#define OP1(expr) { \
	float f1 = pStack->Top(); pStack->Pop(); \
	pStack->Push( expr ); \
	break; \
	}

#define OP2(expr) { \
	float f2 = pStack->Top(); pStack->Pop(); \
	float f1 = pStack->Top(); pStack->Pop(); \
	pStack->Push( expr ); \
	break; \
	}

#define OP2Bool(expr) { \
	float f2 = pStack->Top(); pStack->Pop(); \
	float f1 = pStack->Top(); pStack->Pop(); \
	pStack->Push( (expr) ? 1 : 0); \
	break; \
	}

#define OP3(expr) { \
	float f3 = pStack->Top(); pStack->Pop(); \
	float f2 = pStack->Top(); pStack->Pop(); \
	float f1 = pStack->Top(); pStack->Pop(); \
	pStack->Push( expr ); \
	break; \
	}

#define OP4(expr) { \
	float f4 = pStack->Top(); pStack->Pop(); \
	float f3 = pStack->Top(); pStack->Pop(); \
	float f2 = pStack->Top(); pStack->Pop(); \
	float f1 = pStack->Top(); pStack->Pop(); \
	pStack->Push( expr ); \
	break; \
	}

#define OP5(expr) { \
	float f5 = pStack->Top(); pStack->Pop(); \
	float f4 = pStack->Top(); pStack->Pop(); \
	float f3 = pStack->Top(); pStack->Pop(); \
	float f2 = pStack->Top(); pStack->Pop(); \
	float f1 = pStack->Top(); pStack->Pop(); \
	pStack->Push( expr ); \
	break; \
	}

#define BEGIN_FUNC(token)								\
		case token: {									\
			tFunction funcId = ReadFunc( pInstr );		\
			switch ( funcId ) {
#define END_FUNC()										\
			}											\
		break;											\
		}

float ramp( float x, float a, float b )
{
	return ( x - a ) / ( b - a );
}

float lerp( float x, float a, float b )
{
	return a + x * ( b - a );
}

float smoothstep( float x )
{
	return 3 * x*x - 2 * x*x*x;
}

float CExpressionCalculator::Eval( CUtlStack<float>* pStack, uint8* pInstr, uint8* pInstrEnd, float* pVariables, int nVariables )
{
	Assert( pStack->Count() == 0 );
	pStack->Clear();

	for ( ;; )
	{
		Assert( pInstr < pInstrEnd );

		switch( *pInstr++ )
		{
		case OP_OUTPUT: {
			// Always the last instruction
			Assert( pInstr == pInstrEnd );
			Assert( pStack->Count() == 1 );

			float result = pStack->Top();
			pStack->Pop();

			return result;
		};

		case OP_NOP: break;

		case OP_PUSHVAR: {
			int varIdx = ReadVar( pInstr );
			Assert( varIdx >= 0 && varIdx < nVariables );
			pStack->Push( pVariables[varIdx] );
			break;
		}

		case OP_PUSHLITERAL: {
			float lit = ReadLiteral( pInstr );
			pStack->Push( lit );
			break;
		}

		case OP_NEGATE:		OP1( -f1 );
		case OP_NOT:		OP1( ( f1 == 0 ) ? 1 : 0 );
		case OP_ADD:		OP2( f1 + f2 );
		case OP_SUB:		OP2( f1 - f2 );
		case OP_MUL:		OP2( f1 * f2 );
		case OP_DIV:		OP2( f1 / f2 );
		case OP_MOD:		OP2( fmod( f1, f2 ) );
		case OP_AND:		OP2Bool( ( f1 != 0 ) && ( f2 != 0 ) );
		case OP_OR:			OP2Bool( ( f1 != 0 ) || ( f2 != 0 ) );
		case OP_TERNARY:	OP3( f1 ? f2 : f3 );
		case OP_EQ:			OP2Bool( f1 == f2 );
		case OP_NEQ:		OP2Bool( f1 != f2 );
		case OP_GT:			OP2Bool( f1 > f2 );
		case OP_LT:			OP2Bool( f1 < f2 );
		case OP_GTE:		OP2Bool( f1 >= f2 );
		case OP_LTE:		OP2Bool( f1 <= f2 );
		
		BEGIN_FUNC( OP_FUNC1 )
		case FUNC_ABS:		OP1( fabsf( f1 ) );
		case FUNC_SQR:		OP1( f1 * f1 );
		case FUNC_SQRT:		OP1( sqrtf( f1 ) );
		case FUNC_SIN:		OP1( sinf( f1 ) );
		case FUNC_ASIN:		OP1( asinf( f1 ) );
		case FUNC_COS:		OP1( cosf( f1 ) );
		case FUNC_ACOS:		OP1( acosf( f1 ) );
		case FUNC_TAN:		OP1( tanf( f1 ) );
		case FUNC_EXP:		OP1( expf( f1 ) );
		case FUNC_LOG:		OP1( logf( f1 ) );
		case FUNC_DTOR:		OP1( DEG2RAD( f1 ) );
		case FUNC_RTOD:		OP1( RAD2DEG( f1 ) );
		case FUNC_FLOOR:	OP1( floorf( f1 ) );
		case FUNC_CEILING:	OP1( ceilf( f1 ) );
		case FUNC_ROUND:	OP1( roundf( f1 ) );
		case FUNC_SIGN:		OP1( f1 >= 0.0f ? 1.0f : -1.0f );
		END_FUNC()

		BEGIN_FUNC( OP_FUNC2 )
		case FUNC_MIN:		OP2( MIN( f1, f2 ) );
		case FUNC_MAX:		OP2( MAX( f1, f2 ) );
		case FUNC_ATAN2:	OP2( atan2f( f1, f2 ) );
		case FUNC_POW:		OP2( powf( f1, f2 ) );
		END_FUNC()

		BEGIN_FUNC( OP_FUNC3 )
		case FUNC_INRANGE:	OP3( ( ( f2 <= f1 ) && ( f1 <= f3 ) ) ? 1 : 0 );
		case FUNC_CLAMP:	OP3( clamp( f1, f2, f3 ) );
		case FUNC_RAMP:		OP3( ramp( f1, f2, f3 ) );
		case FUNC_LERP:		OP3( lerp( f1, f2, f3 ) );
		case FUNC_CRAMP:	OP3( clamp( ramp( f1, f2, f3 ), 0, 1 ) );
		case FUNC_CLERP:	OP3( lerp( clamp(f1, 0, 1), f2, f3 ) );
		case FUNC_ELERP:	OP3( lerp( smoothstep( f1 ), f2, f3 ) );
		case FUNC_NOISE:	OP3( ImprovedPerlinNoise( Vector( f1, f2, f3 ) ) );
		END_FUNC()

		// BEGIN_FUNC( OP_FUNC4 )
		// END_FUNC()

		BEGIN_FUNC( OP_FUNC5 )
		case FUNC_RESCALE:	OP5( lerp( ramp( f1, f2, f3 ), f4, f5 ) );
		case FUNC_CRESCALE: OP5( lerp( clamp( ramp( f1, f2, f3 ), 0, 1 ), f4, f5 ) );
		END_FUNC()

		default:
			Assert( false );
			return 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
// Builds a list of variable names from the expression
//-----------------------------------------------------------------------------
bool CExpressionCalculator::BuildVariableListFromExpression( )
{
	ValidateExpr();
	return !m_instrs.IsEmpty();
}

void CExpressionCalculator::ValidateExpr()
{
	if ( m_bExprParsed )
		return;

	m_bExprParsed = true;

	// Clear state
	m_varNames.RemoveAll();
	m_varValues.RemoveAll();
	m_instrs.RemoveAll();

	// Parse the experssion
	const char *expr = m_expr.Get();
	bool success = ParseExpr( expr );

	// Make sure we parsed the whole expression (otherwise an expression like 5 + 3 2 + 6 parses as just "5 + 3")
	if ( success )
	{
		while ( ( *expr == ' ' ) || ( *expr == '\t' ) )
			expr++;

		if ( *expr != '\0' )
			success = false;
	}

	if ( success )
	{
		AddOperator( OP_OUTPUT );
		return;
	}

	// failed to parse.  Make sure state is clear
	m_instrs.RemoveAll();
	m_varNames.RemoveAll();
	m_varValues.RemoveAll();
}

//-----------------------------------------------------------------------------
// Iterate over variables
//-----------------------------------------------------------------------------
int CExpressionCalculator::VariableCount()
{
	return m_varNames.Count();
}

const char *CExpressionCalculator::VariableName( int nIndex )
{
	return m_varNames[nIndex];
}



bool CExpressionCalculator::ParseExpr( const char *&expr )
{
	return ( expr != NULL ) && ParseConditional( expr );
}

bool CExpressionCalculator::ParseConditional( const char *&expr )
{
	ParseState_t ps0( m_instrs, expr );
	if ( !ParseOr( expr ) )
	{
		ps0.Reset( m_instrs, expr );
		return false; // nothing matched
	}

	ParseState_t ps1( m_instrs, expr );
	if ( ParseString( expr, "?" ) &&
		ParseExpr( expr ) &&
		ParseString( expr, ":" ) &&
		ParseExpr( expr ) )
	{
		AddOperator( OP_TERNARY );
		return true; // ?: operator matched
	}

	ps1.Reset( m_instrs, expr );
	return true; // lower precedence expr matched
}

bool CExpressionCalculator::ParseOr( const char *&expr )
{
	ParseState_t ps0( m_instrs, expr );
	if ( !ParseAnd( expr ) )
	{
		ps0.Reset( m_instrs, expr );
		return false; // nothing matched
	}

	ParseState_t ps1( m_instrs, expr );
	if ( ParseString( expr, "||" ) &&
		ParseOr( expr ) )
	{
		AddOperator( OP_OR );
		return true; // || operator matched
	}
	ps1.Reset( m_instrs, expr );
	return true; // lower precedence expr matched
}

bool CExpressionCalculator::ParseAnd( const char *&expr )
{
	ParseState_t ps0( m_instrs, expr );
	if ( !ParseEquality( expr ) )
	{
		ps0.Reset( m_instrs, expr );
		return false; // nothing matched
	}

	ParseState_t ps1( m_instrs, expr );
	if ( ParseString( expr, "&&" ) &&
		ParseAnd( expr ) )
	{
		AddOperator( OP_AND );
		return true; // && operator matched
	}
	ps1.Reset( m_instrs, expr );
	return true; // lower precedence expr matched
}

bool CExpressionCalculator::ParseEquality( const char *&expr )
{
	static const char *pOps[] = { "==", "!=" };
	static tOperator oOps[] = { OP_EQ, OP_NEQ };
	COMPILE_TIME_ASSERT( ARRAYSIZE( pOps ) == ARRAYSIZE( oOps ) );
	int nOp = ARRAYSIZE( pOps );

	ParseState_t ps0( m_instrs, expr );
	if ( !ParseLessGreater( expr ) )
	{
		ps0.Reset( m_instrs, expr );
		return false; // nothing matched
	}

	ParseState_t ps1( m_instrs, expr );
	if ( ParseStringList( expr, pOps, nOp ) &&
		ParseEquality( expr ) )
	{
		AddOperator( oOps[nOp] );
		return true; // equality expression matched
	}
	ps1.Reset( m_instrs, expr );
	return true; // lower precedence expr matched
}

bool CExpressionCalculator::ParseLessGreater( const char *&expr )
{
	static const char *pOps[] = { "<", ">", "<=", ">=" };
	static tOperator oOps[] = { OP_LT, OP_GT, OP_LTE, OP_GTE };
	COMPILE_TIME_ASSERT( ARRAYSIZE( pOps ) == ARRAYSIZE( oOps ) );

	ParseState_t ps0( m_instrs, expr );
	if ( !ParseAddSub( expr ) )
	{
		ps0.Reset( m_instrs, expr );
		return false; // nothing matched
	}

	ParseState_t ps1( m_instrs, expr );
	int nOp = ARRAYSIZE( pOps );
	if ( ParseStringList( expr, pOps, nOp ) &&
		ParseLessGreater( expr ) )
	{
		AddOperator( oOps[nOp] );
		return true; // comparison expression matched
	}
	ps1.Reset( m_instrs, expr );
	return true; // lower precedence expr matched
}

bool CExpressionCalculator::ParseAddSub( const char *&expr )
{
	static const char *pOps[] = { "+", "-" };
	static tOperator oOps[] = { OP_ADD, OP_SUB };
	COMPILE_TIME_ASSERT( ARRAYSIZE( pOps ) == ARRAYSIZE( oOps ) );

	ParseState_t ps0( m_instrs, expr );
	if ( !ParseDivMul( expr ) )
	{
		ps0.Reset( m_instrs, expr );
		return false; // nothing matched
	}

	ParseState_t ps1( m_instrs, expr );
	int nOp = ARRAYSIZE( pOps );
	if ( ParseStringList( expr, pOps, nOp ) &&
		ParseAddSub( expr ) )
	{
		AddOperator( oOps[nOp] );
		return true; // addsub matched
	}
	ps1.Reset( m_instrs, expr );
	return true; // lower precedence expr matched
}

bool CExpressionCalculator::ParseDivMul( const char *&expr )
{
	static const char *pOps[] = { "*", "/", "%" };
	static tOperator oOps[] = { OP_MUL, OP_DIV, OP_MOD };
	COMPILE_TIME_ASSERT( ARRAYSIZE( pOps ) == ARRAYSIZE( oOps ) );

	ParseState_t ps0( m_instrs, expr );
	if ( !ParseUnary( expr ) )
	{
		ps0.Reset( m_instrs, expr );
		return false; // nothing matched
	}

	ParseState_t ps1( m_instrs, expr );
	int nOp = ARRAYSIZE( pOps );
	if ( ParseStringList( expr, pOps, nOp ) &&
		ParseDivMul( expr ) )
	{
		AddOperator( oOps[nOp] );
		return true; // divmul matched
	}
	ps1.Reset( m_instrs, expr );
	return true; // lower precedence expr matched
}

bool CExpressionCalculator::ParseUnary( const char *&expr )
{
	static const char *pOps[] = { "+", "-", "!" };
	static tOperator oOps[] = { OP_NOP, OP_NEGATE, OP_NOT };
	COMPILE_TIME_ASSERT( ARRAYSIZE( pOps ) == ARRAYSIZE( oOps ) );

	ParseState_t ps( m_instrs, expr );
	int nOp = ARRAYSIZE( pOps );
	if ( ParseStringList( expr, pOps, nOp ) &&
		ParseUnary( expr ) )
	{
		// Don't add useless NOP for unary +
		if ( oOps[nOp] != OP_NOP )
			AddOperator( oOps[nOp] );

		return true;
	}

	ps.Reset( m_instrs, expr );
	if ( ParsePrimary( expr ) )
		return true;

	ps.Reset( m_instrs, expr );
	return false;
}

bool CExpressionCalculator::ParsePrimary( const char *&expr )
{
	ParseState_t ps( m_instrs, expr );

	float value = 0.0f;
	if ( ParseLiteral( expr, value ) )
	{
		AddOperator( OP_PUSHLITERAL );
		AddLiteral( value );
		return true;
	}

	ps.Reset( m_instrs, expr );
	if ( ParseString( expr, "(" ) &&
		ParseExpr( expr ) &&
		ParseString( expr, ")" ) )
	{
		return true;
	}

	ps.Reset( m_instrs, expr );
	if ( Parse1ArgFunc( expr ) ||
		Parse2ArgFunc( expr ) ||
		Parse3ArgFunc( expr ) ||
		//		Parse4ArgFunc( expr ) ||
		Parse5ArgFunc( expr ) )
	{
		return true;
	}

	ps.Reset( m_instrs, expr );
	if ( ParseVariable( expr ) )
	{
		return true;
	}

	ps.Reset( m_instrs, expr );
	return false;
}

bool CExpressionCalculator::Parse1ArgFunc( const char *&expr )
{
	static const char *pFuncs[] =
	{
		"abs", "sqr", "sqrt", "sin", "asin", "cos", "acos", "tan",
		"exp", "log", "dtor", "rtod", "floor", "ceiling", "round", "sign"
	};
	static const tFunction oFuncs[] =
	{
		FUNC_ABS, FUNC_SQR, FUNC_SQRT, FUNC_SIN, FUNC_ASIN, FUNC_COS, FUNC_ACOS, FUNC_TAN,
		FUNC_EXP, FUNC_LOG, FUNC_DTOR, FUNC_RTOD, FUNC_FLOOR, FUNC_CEILING, FUNC_ROUND, FUNC_SIGN
	};
	COMPILE_TIME_ASSERT( ARRAYSIZE( pFuncs ) == ARRAYSIZE( oFuncs ) );

	ParseState_t ps( m_instrs, expr );
	int nFunc = ARRAYSIZE( pFuncs );
	if ( ParseStringList( expr, pFuncs, nFunc ) &&
		ParseString( expr, "(" ) &&
		ParseExpr( expr ) &&
		ParseString( expr, ")" ) )
	{
		AddOperator( OP_FUNC1 );
		AddFunc( oFuncs[nFunc] );
		return true;
	}

	ps.Reset( m_instrs, expr );
	return false;
}

bool CExpressionCalculator::Parse2ArgFunc( const char *&expr )
{
	static const char *pFuncs[] = { "min", "max", "atan2", "pow" };
	static const tFunction oFuncs[] = { FUNC_MIN, FUNC_MAX, FUNC_ATAN2, FUNC_POW };
	COMPILE_TIME_ASSERT( ARRAYSIZE( pFuncs ) == ARRAYSIZE( oFuncs ) );

	ParseState_t ps( m_instrs, expr );
	int nFunc = ARRAYSIZE( pFuncs );
	if ( ParseStringList( expr, pFuncs, nFunc ) &&
		ParseString( expr, "(" ) &&
		ParseExpr( expr ) &&
		ParseString( expr, "," ) &&
		ParseExpr( expr ) &&
		ParseString( expr, ")" ) )
	{
		AddOperator( OP_FUNC2 );
		AddFunc( oFuncs[nFunc] );
		return true;
	}

	ps.Reset( m_instrs, expr );
	return false;
}

bool CExpressionCalculator::Parse3ArgFunc( const char *&expr )
{
	static const char *pFuncs[] = { "inrange", "clamp", "ramp", "lerp", "cramp", "clerp", "elerp", "noise" };
	static const tFunction oFuncs[] = { FUNC_INRANGE, FUNC_CLAMP, FUNC_RAMP, FUNC_LERP, FUNC_CRAMP, FUNC_CLERP, FUNC_ELERP, FUNC_NOISE };
	COMPILE_TIME_ASSERT( ARRAYSIZE( pFuncs ) == ARRAYSIZE( oFuncs ) );

	ParseState_t ps( m_instrs, expr );
	int nFunc = ARRAYSIZE( pFuncs );
	if ( ParseStringList( expr, pFuncs, nFunc ) &&
		ParseString( expr, "(" ) &&
		ParseExpr( expr ) &&
		ParseString( expr, "," ) &&
		ParseExpr( expr ) &&
		ParseString( expr, "," ) &&
		ParseExpr( expr ) &&
		ParseString( expr, ")" ) )
	{
		AddOperator( OP_FUNC3 );
		AddFunc( oFuncs[nFunc] );
		return true;
	}

	ps.Reset( m_instrs, expr );
	return false;
}

//bool CExpressionCalculator::Parse4ArgFunc( const char *&expr );

bool CExpressionCalculator::Parse5ArgFunc( const char *&expr )
{
	static const char *pFuncs[] = { "rescale", "crescale" };
	static const tFunction oFuncs[] = { FUNC_RESCALE, FUNC_CRESCALE };
	COMPILE_TIME_ASSERT( ARRAYSIZE( pFuncs ) == ARRAYSIZE( oFuncs ) );

	ParseState_t ps( m_instrs, expr );
	int nFunc = ARRAYSIZE( pFuncs );
	if ( ParseStringList( expr, pFuncs, nFunc ) &&
		ParseString( expr, "(" ) &&
		ParseExpr( expr ) &&
		ParseString( expr, "," ) &&
		ParseExpr( expr ) &&
		ParseString( expr, "," ) &&
		ParseExpr( expr ) &&
		ParseString( expr, "," ) &&
		ParseExpr( expr ) &&
		ParseString( expr, "," ) &&
		ParseExpr( expr ) &&
		ParseString( expr, ")" ) )
	{
		AddOperator( OP_FUNC5 );
		AddFunc( oFuncs[nFunc] );
		return true;
	}

	ps.Reset( m_instrs, expr );
	return false;
}

bool CExpressionCalculator::ParseVariable( const char *&exprStart )
{
	const char* expr = exprStart;

	// Skip whitespace
	while ( *expr == ' ' || *expr == '\t' )
		++expr;

	// Note:
	//
	// We don't want to use ParseStringList here to match existing variables
	// because that doesn't tokenize properly; if there is a variable named
	// "a" then "alfalfa" will parse just the "a" leaving an extra "lfalfa"
	// token following which will inevitably fail to parse.
	//
	// This isn't a problem for symbols because our symbol grammar is completely
	// non-ambiguous (no ++ vs + + issue).
	//
	// It also isn't a problem for built-in functions because they immediately
	// parse the following "(" which fails.  For example, "sinusodial" matches
	// the function "sin", but doesn't match "sin" + "(" so it fails.
	//
	// Variables have no such luxury as we don't "lookahead" for the next token.
	// Instead we tokenize the entire variable ourselves here, and compare to
	// each known variable name.

	// Variables can't start with a number
	if ( V_isdigit( *expr ) )
		return false;

	const char *pStart = expr;

	// CSGO: Our quest definitions use variables of the form %varname%
	//       Adding these doesn't make the grammar ambiguous as % cannot
	//       appear in a variable position (the only symbols that can
	//       appear there are +, -, and !, for unary expressions).  We
	//       only match the trailing '%' in the case that the starting %
	//       was matched.
	char varSymbol = 0;
	if ( *expr == '%' ) // could add more symbols here, like $, @, *, etc.  Anything except +-!(),
	{
		varSymbol = *expr;
		++expr;
	}

	while ( V_isalnum( *expr ) || *expr == '_' )
	{
		++expr;
	}

	// Match trailing symbol that matches starting symbol for variable.
	if ( varSymbol != 0 )
	{
		if ( *expr != varSymbol )
			return false;

		++expr;
	}

	size_t nLen = ( size_t )expr - ( size_t )pStart;

	// Make sure we successfully parsed an identifier
	if ( nLen == 0 )
		return false;

	// Find or add the variable with this name
	int varIdx = -1;
	FOR_EACH_VEC( m_varNames, iVar )
	{
		const char* varName = m_varNames[iVar];
		if ( !strnicmp( varName, pStart, nLen )	// strings match at least up to nLen
			&& varName[nLen] == '\0'			// strings have the same length
			)
		{
			varIdx = iVar;
			break;
		}
	}

	// If we didn't find it, add a new variable with this name
	if ( varIdx == -1 )
	{
		char *pVariableName = ( char* )stackalloc( nLen + 1 );
		memcpy( pVariableName, pStart, nLen );
		pVariableName[nLen] = 0;

		// Add the variable to our list of variables
		Assert( m_varValues.Count() == m_varNames.Count() );
		varIdx = m_varNames.AddToTail( pVariableName );
		int valueIdx = m_varValues.AddToTail( 0.0f );
		Assert( varIdx == valueIdx );
	}

	// Compile into PUSHVAR instruction and update parse state
	exprStart = expr;
	AddOperator( OP_PUSHVAR );
	AddVar( varIdx );
	return true;
}

CExpressionCalculator::CExpressionCalculator( const CExpressionCalculator& x )
{
	*this = x;
}

CExpressionCalculator& CExpressionCalculator::operator=( const CExpressionCalculator& x )
{
	m_bExprParsed = x.m_bExprParsed;
	m_expr = x.m_expr;
	m_instrs = x.m_instrs;
	m_varNames = x.m_varNames;
	m_varValues = x.m_varValues;
	m_stack.CopyFrom( x.m_stack );
	return *this;
}

float EvaluateExpression( char const *pExpr, float flValueToReturnIfFailure )
{
	CExpressionCalculator myEvaluator( pExpr );
	float flResult;
	bool bSuccess = myEvaluator.Evaluate( flResult );
	return ( bSuccess ) ? flResult : flValueToReturnIfFailure;
}

