//====== Copyright c 1996-2008, Valve Corporation, All rights reserved. =======

#ifndef MATHLIB_EXPRESSION_CALCULATOR_H
#define MATHLIB_EXPRESSION_CALCULATOR_H
#ifdef _WIN32
#pragma once
#endif

#include "tier1/utlstring.h"
#include "tier1/utlstack.h"
#include "tier1/utlvector.h"


#define EXPRESSIONCALCULATOR_USE_EXPRESSIVE_PARSE 0

//-----------------------------------------------------------------------------
// Calculator Parsing class
// precedence order:
//		unary operators: + - ! func var
//		* / %
//		+ -
//		< > <= >=
//		== !=
//		&&
//		||
//		?:
//-----------------------------------------------------------------------------
class CExpressionCalculator
{
public:
	CExpressionCalculator( const char *expr = NULL ) : m_bExprParsed(false), m_expr( expr ) {}

	CExpressionCalculator( const CExpressionCalculator& x );
	CExpressionCalculator& operator=( const CExpressionCalculator& x );
	
public:
	void SetExpression( const char *expr ) 
	{
		m_expr = expr;
		m_bExprParsed = false;
	}

	void SetVariable( const char *var, float value );
	void SetVariable( int nVariableIndex, float value );
	void ModifyVariable( const char *var, float value );

	int FindVariableIndex( const char *var );

	bool Evaluate( float &value );

	// Builds a list of variable names from the expression
	bool BuildVariableListFromExpression( );

	// Iterate over variables
	int VariableCount();
	const char *VariableName( int nIndex );

private:
	bool ParseExpr		 ( const char *&expr );
	bool ParseConditional( const char *&expr );
	bool ParseOr		 ( const char *&expr );
	bool ParseAnd		 ( const char *&expr );
	bool ParseEquality	 ( const char *&expr );
	bool ParseLessGreater( const char *&expr );
	bool ParseAddSub	 ( const char *&expr );
	bool ParseDivMul	 ( const char *&expr );
	bool ParseUnary		 ( const char *&expr );
	bool ParsePrimary	 ( const char *&expr );
	bool Parse1ArgFunc	 ( const char *&expr );
	bool Parse2ArgFunc	 ( const char *&expr );
	bool Parse3ArgFunc	 ( const char *&expr );
	//	bool Parse4ArgFunc	 ( const char *&expr );
	bool Parse5ArgFunc	 ( const char *&expr );
	bool ParseVariable   ( const char *&expr );

#if EXPRESSIONCALCULATOR_USE_EXPRESSIVE_PARSE
#define READABLE(x) = x
#else
#define READABLE(x)
#endif

	enum tOperator : uint8 {
		OP_INVALID		= 0xff,		// not used
		OP_OUTPUT		= 0,		// always the last instruction

		OP_NOP			READABLE('N'),
		OP_PUSHVAR		READABLE('V'),
		OP_PUSHLITERAL	READABLE('L'),

		OP_NEGATE		READABLE('n'),
		OP_NOT			READABLE('!'),
		OP_ADD			READABLE('+'),
		OP_SUB			READABLE('-'),
		OP_MUL			READABLE('*'),
		OP_DIV			READABLE('/'),
		OP_MOD			READABLE('%'),
		OP_AND			READABLE('&'),
		OP_OR			READABLE('|'),
		OP_TERNARY		READABLE('?'),		// C-style ?: operator.  Note: calculates both sides regardless of result of condition.  Would need branching to avoid this.

		OP_EQ			READABLE('='),
		OP_NEQ			READABLE('w'),
		OP_GT			READABLE('>'),
		OP_LT			READABLE('<'),
		OP_GTE			READABLE(','),
		OP_LTE			READABLE('.'),

		// functions
		OP_FUNC1		READABLE('1'),
		OP_FUNC2		READABLE('2'),
		OP_FUNC3		READABLE('3'),
		//OP_FUNC4		READABLE('4'),
		OP_FUNC5		READABLE('5'),
	};

#if EXPRESSIONCALCULATOR_USE_EXPRESSIVE_PARSE
	enum tFunction : uint32
#else
	enum tFunction : uint8
#endif
	{
		// 1-argument functions
		FUNC_ABS		READABLE('Abs '),		// abs(x), absolute value
		FUNC_SQR		READABLE('Sqr '),		// sqr(x), x^2
		FUNC_SQRT		READABLE('Sqrt'),		// sqrt(x)
		
		FUNC_SIN		READABLE('Sin '),		// sin(x)
		FUNC_ASIN		READABLE('Asin'),		// asin(x)
		FUNC_COS		READABLE('Cos '),		// cos(x)
		FUNC_ACOS		READABLE('Acos'),		// acos(x)
		FUNC_TAN		READABLE('Tan '),		// tan(x)
		
		FUNC_EXP		READABLE('Exp '),		// exp(x), e^x
		FUNC_LOG		READABLE('Log '),		// log(X), ln x

		FUNC_DTOR		READABLE('Dtor'),		// dtor(x), degrees to radians
		FUNC_RTOD		READABLE('Rtod'),		// rtod(x), radians to degrees

		FUNC_FLOOR		READABLE('Flor'),		// floor(x)
		FUNC_CEILING	READABLE('Ceil'),		// ceiling(x)
		FUNC_ROUND		READABLE('Roun'),		// round(x)
		FUNC_SIGN		READABLE('Sign'),		// sign(x), -1 if x is negative, 1 otherwise.

		// 2-argument functions
		FUNC_MIN		READABLE('Min '),		// min(x,y)
		FUNC_MAX		READABLE('Max '),		// max(x,y)
		FUNC_ATAN2		READABLE('Atan'),		// atan2(x,y) = atan (y/x), pi/2 when x = 0
		FUNC_POW		READABLE('Pow '),		// pow(x,y) = x^y

		// 3-argument functions
		FUNC_INRANGE	READABLE('Rang'),		// inrange(x,min,max), returns 1 if min <= x <= max, otherwise 0
		FUNC_CLAMP		READABLE('Clmp'),		// clamp(x,min,max) clamps x to the range [min,max]
		FUNC_RAMP		READABLE('Ramp'),		// ramp(x,a,b) inverse lerp, goes from 0->1 as x goes from a->b
		FUNC_LERP		READABLE('Lerp'),		// lerp(x,a,b) lerp, goes from a->b as x goes from 0->1
		FUNC_CRAMP		READABLE('Crmp'),		// cramp(x,a,b) = clamp( ramp(x,a,b), 0, 1 )
		FUNC_CLERP		READABLE('Clrp'),		// clerp(x,a,b) = clamp( lerp(x,a,b), a, b )
		FUNC_ELERP		READABLE('Elrp'),		// elerp(x,a,b) = ramp( 3x^2 - 2x^3, a, b )  (spline)
		FUNC_NOISE		READABLE('Nois'),		// noise(a,b,c) = sample 3-d perlin noise

		// 4-argument functions

		// 5-argument functions
		FUNC_RESCALE	READABLE('Rscl'),		// rescale(X, Xa, Xb, Ya, Yb) goes from Ya->Yb as X goes from Xa->Xb.  lerp(ramp(X,Xa,Xb),Ya,Yb)
		FUNC_CRESCALE	READABLE('CRsc'),		// crescale(X, Xa, Xb, Ya, Yb) Same as above, but clamped. lerp(cramp(X,Xa,Xb),Ya,Yb)
	};

	void AddOperator( tOperator op );
	void AddFunc( tFunction func );
	void AddVar( int varIndex );
	void AddLiteral( float lit );

	static tOperator ReadOperator( uint8* &instr );
	static tFunction ReadFunc( uint8* &instr );
	static int ReadVar( uint8* &instr );
	static float ReadLiteral( uint8* &instr );

	void ValidateExpr(); // Parse m_expr into m_instrs if it hasn't been parsed yet
	static float Eval( CUtlStack<float>* pStack, uint8* pInstr, uint8* pInstrEnd, float* pVariables, int nVariables );

#undef READABLE

	bool m_bExprParsed;
	CUtlString m_expr;
	CUtlVector<uint8> m_instrs; // parsed version of m_expr
	CUtlVector< CUtlString > m_varNames;
	CUtlVector<float> m_varValues;
	CUtlStack<float> m_stack;
};

// simple warppers for using cExpressionCalculator
float EvaluateExpression( char const *pExprString, float flValueToReturnIfFailure );


#endif // MATHLIB_EXPRESSION_CALCULATOR_H
