# ==============================================================================
# Generated by qmake (2.01a) (Qt 4.6.0) on: Wed Jan 6 18:50:35 2010
# This file is generated by qmake and should not be modified by the
# user.
#  Name        : Makefile_0xE6A4C06A.mk
#  Part of     : aciedit
#  Description : This file is used to call necessary targets on wrapper makefile
#                during normal Symbian build process.
#  Version     : 
#
# ==============================================================================



MAKE = make

VISUAL_CFG = RELEASE
ifeq "$(CFG)" "UDEB"
VISUAL_CFG = DEBUG
endif

do_nothing :
	@rem do_nothing

MAKMAKE: create_temps pre_targetdeps store_build

LIB: create_temps pre_targetdeps store_build

BLD: create_temps pre_targetdeps store_build

ifeq "$(PLATFORM)" "WINSCW"
CLEAN: extension_clean winscw_deployment_clean
else
CLEAN: extension_clean
endif

CLEANLIB: do_nothing

RESOURCE: do_nothing

FREEZE: do_nothing

SAVESPACE: do_nothing

RELEASABLES: do_nothing

ifeq "$(PLATFORM)" "WINSCW"
FINAL: finalize winscw_deployment
else
FINAL: finalize
endif

pre_targetdeps : c:\Symbian\Carbide\workspace\aciedit\Makefile
	-$(MAKE) -f "c:\Symbian\Carbide\workspace\aciedit\Makefile" pre_targetdeps QT_SIS_TARGET=$(VISUAL_CFG)-$(PLATFORM)

create_temps : c:\Symbian\Carbide\workspace\aciedit\Makefile
	-$(MAKE) -f "c:\Symbian\Carbide\workspace\aciedit\Makefile" create_temps QT_SIS_TARGET=$(VISUAL_CFG)-$(PLATFORM)

extension_clean : c:\Symbian\Carbide\workspace\aciedit\Makefile
	-$(MAKE) -f "c:\Symbian\Carbide\workspace\aciedit\Makefile" extension_clean QT_SIS_TARGET=$(VISUAL_CFG)-$(PLATFORM)

finalize : c:\Symbian\Carbide\workspace\aciedit\Makefile
	-$(MAKE) -f "c:\Symbian\Carbide\workspace\aciedit\Makefile" finalize QT_SIS_TARGET=$(VISUAL_CFG)-$(PLATFORM)

winscw_deployment_clean : c:\Symbian\Carbide\workspace\aciedit\Makefile
	-$(MAKE) -f "c:\Symbian\Carbide\workspace\aciedit\Makefile" winscw_deployment_clean QT_SIS_TARGET=$(VISUAL_CFG)-$(PLATFORM)

winscw_deployment : c:\Symbian\Carbide\workspace\aciedit\Makefile
	-$(MAKE) -f "c:\Symbian\Carbide\workspace\aciedit\Makefile" winscw_deployment QT_SIS_TARGET=$(VISUAL_CFG)-$(PLATFORM)

store_build : c:\Symbian\Carbide\workspace\aciedit\Makefile
	-$(MAKE) -f "c:\Symbian\Carbide\workspace\aciedit\Makefile" store_build QT_SIS_TARGET=$(VISUAL_CFG)-$(PLATFORM)


