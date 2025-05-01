@echo off

::
:: solo
::
  :: family 15h
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Br   -p Am4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: family 17h, models 00 - 0F
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Sm   -p Am4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Sm   -p Sp4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Zp   -p Sp3   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Zp   -p Sp4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Tr   -p Sp3   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: family 17h, models 10 - 2F
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Rv   -p Am4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Rv   -p Fp5   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: family 17h, models 30 - 3F
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Ssp  -p Sp3   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Cp   -p Sp3   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: family 17h, models 60 - 6F
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Rn   -p Am4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Rn   -p Fp6   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: family 17h, models 70 - 7F
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Mts  -p Am4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: family 17h, models 20h-2Fh
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Rv  -p Ft5   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: family 17h, models 90h-97h
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Vn  -p Ff3   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: family 17h, models 98h-9Fh
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Mr  -p Ff3   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: family 17h, models
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Ar  -p Ff3   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: family 17h, models A0h-AFh
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Mdn  -p Ft6  -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: family 19h, models 00 - 0F
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Vmr  -p Am4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: family 19h, models 10 - 1F
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Rs  -p Sp5   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: family 19h, models 20 - 2F
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Gn  -p Sp3   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: family 19h, models 30 - 3F
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Ba  -p Sp3   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: family 19h, models 40 - 4F
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Rmb  -p Am5   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Rmb  -p Fp7   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Rmb  -p Fp7r2 -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: family 19h, models 50 - 5F
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Czn  -p Am4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Czn  -p Fp6   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: family 19h, models 60 - 6F
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Rpl  -p Am5   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: family 19h, models 70 - 7F
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Phx  -p Fp8   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: family 19h, models 80 - 8F
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o MI3  -p Sh5   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: family 19h, models 10 - 1F
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Stp  -p Sp6   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o Stp  -p Tr5   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"
::
:: combo
::
  :: AM5 combo
    :: 2 combo
      perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o "Rpl|Rmb"                    -p Am5   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"
	  
  :: AM4 combo
    :: 2 combo
      perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o "Sm|Br"                      -p Am4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"
      perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o "Rv|Br"                      -p Am4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"
      perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o "Rv|Sm"                      -p Am4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"
      perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o "Rn|Mts"                     -p Am4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"
      perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o "Rn|Rv"                      -p Am4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"
      perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o "Vmr|Mts"                    -p Am4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"
      perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o "Vmr|Rn"                     -p Am4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"
      perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o "Czn|Rn"                     -p Am4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

    :: 3 combo
      perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o "Rv|Sm|Br"                   -p Am4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"
      perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o "Mts|Rv|Sm"                  -p Am4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"
      perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o "Vmr|Rn|Mts"                 -p Am4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

    :: 4 combo
      perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o "Czn|Vmr|Rn|Mts"             -p Am4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

    :: 5 combo
      perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o "Rn|Mts|Rv|Sm|Br"            -p Am4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

    :: 6 combo - No Br
      perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o "Czn|Vmr|Rn|Mts|Rv|Sm"       -p Am4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

    :: All combo
      perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o "Czn|Vmr|Rn|Mts|Rv|Sm|Br"    -p Am4   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: FP6 combo
    :: 2 combo
      perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o "Czn|Rn"                     -p Fp6   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: SP3 combo
    :: 2 combo
      perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o "Ssp|Zp"                     -p Sp3   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  :: SP3r2 combo
    :: All combo
      perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o "Cp|Tr"                      -p Sp3   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

  ::SP3r3 combo
    :: 2 combo
      perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o "Cgl|Cp"                     -p Sp3   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

::
:: no platform
::
  :: BXB
    perl "%CD%\AMDTools\ArchV9\ArchV9.pl" -o BXB  -p NULL   -i "%CD%\AMDTools\ArchV9\Config" -e "%CD%"

@echo on
