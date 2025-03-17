# VR Stuff

Gmod has an option `-vr` which will attempt to load a dll `headtrack_oculus.dll` and will try to load the exposed `"VHeadTrack001"` interface.  

> [!NOTE]
> The `headtrack_oculus.dll` was removed from gmod a good while ago, it still can be found in old builds.  
> Gmod's headtrack version seems to be mostly unknown as there isn't even any leaked source of it BUT it seems like many of it's functions were later split into `CClientVirtualReality`.  