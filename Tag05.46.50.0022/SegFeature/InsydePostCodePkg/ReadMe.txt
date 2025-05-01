InsydePostCodePkg uses the "SmmCorePlatformHookLib" to configure the debug registers when processor enters SMM.
If your platform also uses this hook library to do something, please follow the following steps to modify your "SmmCorePlatformHookLib" instance.

1.Remove the "SmmCorePlatformHookLib|InsydePostCodePkg/Library/SmmCorePlatformHookLib/SmmCorePlatformHookLib.inf" statement, which is in the "InsydePostCodePkg/Package.dsc".
  
2.InsydePostCodePkg provides a "SmmDebugRegisterConfigLib" library to configure the debug registers.
  Include this library and call the "SmmDebugRegisterConfig ()" in the "PlatformHookBeforeSmmDispatch ()" of your "SmmCorePlatformHookLib" instance.