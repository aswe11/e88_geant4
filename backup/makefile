#makefile for E16G4 by S. Yokkaichi
#    Last modified at <2017-09-06 17:37:19 >
#memo
# setenv E16SYS /ccj/u/E16; source $E16SYS/bin/E16setup.csh

#--------------------------------
# library/include path section
#

# Default E16SYS if not set (fallback to CCJ path for backward compatibility)
ifeq ($(E16SYS),)
  E16SYS = /ccj/u/E16
endif

# Detect environment for sub-library versions
# At CCJ, we use 2020-09-16. At KEKCC, this should be set in the setup script.
ifeq ($(E16ANA_VERSION),)
  E16ANA_VERSION = 2020-09-16
endif

ROOTLIBS_DYN = -L$(E16SYS)/lib64 -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic -lMinuit -lGenVector -lSpectrum
ROOTLIBS_STAT = -L$(E16SYS)/lib64 -lRoot -lpcre -llzma -pthread -lz -lm -ldl 

ROOTINCLUDE    = `$(E16SYS)/bin/root-config --cflags`
G4INCLUDE      = $(E16SYS)/include/Geant4
HepPDTINCLUDE  = $(E16SYS)/include/ 

CLHEPLIB = -L$(E16SYS)/lib64 -lG4clhep -lHepPDT -lHepPID

G4LIB    = -L$(E16SYS)/lib64 -lG4visHepRep -lG4vis_management -lG4visXXX -lG4Tree -lG4physicslists -lG4run -lG4tracking -lG4processes -lG4materials -lG4GMocren -lG4modeling -lG4event -lG4digits_hits -lG4geometry  -lG4intercoms -lG4analysis -lG4FR -lG4error_propagation -lG4expat -lG4gl2ps -lG4global -lG4graphics_reps -lG4interfaces -lG4parmodels -lG4particles -lG4persistency -lG4RayTracer -lG4readout -lG4track  -lG4OpenGL  -lG4VRML

3G4LIB    = -L$(E16SYS)/lib64 -lG4visHepRep -lG4vis_management -lG4visXXX -lG4Tree -lG4physicslists -lG4run -lG4tracking -lG4processes -lG4materials -lG4GMocren -lG4modeling -lG4event -lG4digits_hits -lG4geometry  -lG4intercoms -lG4analysis -lG4FR -lG4error_propagation -lG4expat -lG4gl2ps -lG4global -lG4graphics_reps -lG4interfaces -lG4parmodels -lG4persistency -lG4RayTracer -lG4readout -lG4track  -lG4particles  -lG4OpenGL  -lG4VRML -lG4zlib

E16INCLUDE     = $(E16SYS)/E16ANA/$(E16ANA_VERSION)/include
E16ANALIB      = -L$(E16SYS)/E16ANA/$(E16ANA_VERSION)/lib64 -lE16ANA


E16G4LIBlocal   = -L./lib64 -lE16G4

CC = g++
AR = ar
ARFLAGS = rv
SHADOP = -shared -fPIC

INCDIR= -I./include/ -I$(E16INCLUDE) -I$(ROOTINCLUDE) -I$(G4INCLUDE)  -I$(HepPDTINCLUDE)

CCFLAGS = -O3 -Wall -g 
STATFLAGS = -static ${CCFLAGS} 

SRCD    = src/
OBJD    = obj/
DEPENDD = obj/
SRCED    = examples/
OBJED    = obj/


#.cc.o:
#	${CC} ${CCFLAGS} ${INCDIR} -c -MD $< -o $@ ${ROOTLIBS_STAT}



#-------------------------------------------
# macro section

LIB_STAT = lib64/libE16G4.a
LIB_SO   = lib64/libE16G4.so

TARGET1  = E16G4
EXAMPLE1  = E16G4-yok.stat

#sako 2019/10/23
#EXAMPLE2  = E16ANA_SingleFitDevelopment.stat
EXAMPLE3  = multiFitSingle

TARGET2  = g4out2tree


#TARGETS = ${LIB_STAT} ${LIB_SO} \
#	${TARGET1}.stat ${TARGET1}.dyn ${TARGET1}.vis-stat \
#	${EXAMPLE1}  ${TARGET2}.stat 

TARGETS = ${LIB_STAT} ${LIB_SO} \
	${TARGET1}.stat ${TARGET1}.dyn ${TARGET1}.vis-stat \
	${EXAMPLE1}  ${TARGET2}  ${EXAMPLE3}.stat

all: ${TARGETS}
lib: ${LIB_STAT} 

SRCS0 =  $(SRCD)E16G4_Analysis.cc \
         $(SRCD)E16G4_AnalysisMessenger.cc \
         $(SRCD)E16G4_BeamDump.cc \
         $(SRCD)E16G4_BeamLineElement.cc \
         $(SRCD)E16G4_BeamPipe.cc \
         $(SRCD)E16G4_DetectorConstruction.cc \
         $(SRCD)E16G4_DetectorConstructionMessenger.cc \
         $(SRCD)E16G4_EventAction.cc \
         $(SRCD)E16G4_Field.cc \
         $(SRCD)E16G4_FMMagnet.cc \
         $(SRCD)E16G4_GTRFrameHit.cc \
         $(SRCD)E16G4_GTRFrameSD.cc \
         $(SRCD)E16G4_GTRGEM.cc \
         $(SRCD)E16G4_GTRHit.cc \
         $(SRCD)E16G4_GTRSD.cc \
         $(SRCD)E16G4_HBD.cc \
         $(SRCD)E16G4_HBDHit.cc \
         $(SRCD)E16G4_HBDSD.cc \
         $(SRCD)E16G4_LG.cc \
         $(SRCD)E16G4_LGHit.cc \
         $(SRCD)E16G4_LGSD.cc \
         $(SRCD)E16G4_SSD.cc \
         $(SRCD)E16G4_SSDHit.cc \
         $(SRCD)E16G4_SSDSD.cc \
         $(SRCD)E16G4_RPC.cc \
         $(SRCD)E16G4_RPCHit.cc \
         $(SRCD)E16G4_RPCSD.cc \
         $(SRCD)E16G4_MaterialList.cc \
         $(SRCD)E16G4_PhysicsList.cc \
         $(SRCD)E16G4_PrimaryGeneratorAction.cc \
         $(SRCD)E16G4_PrimaryGeneratorMessenger.cc \
         $(SRCD)E16G4_RunAction.cc \
         $(SRCD)E16G4_SteppingAction.cc \
         $(SRCD)E16G4_Target.cc \
         $(SRCD)E16G4_TargetChamber.cc \
         $(SRCD)E16G4_TargetHit.cc \
         $(SRCD)E16G4_TargetSD.cc \
         $(SRCD)E16G4_TrackingAction.cc \
         $(SRCD)E16G4_Trajectory.cc \
         $(SRCD)E16G4_Transportation.cc \
         $(SRCD)E16G4_VD.cc \
         $(SRCD)E16G4_VDCylinder.cc \
         $(SRCD)E16G4_VDCylinderHit.cc \
         $(SRCD)E16G4_VDCylinderSD.cc \
         $(SRCD)E16G4_VDHit.cc \
         $(SRCD)E16G4_VDSD.cc \
         $(SRCD)E16G4_SanfordWangGenerator.cc \
         $(SRCD)E16ANA_GeometryV2.cc \
         $(SRCD)E16ANA_G4OutputData.cc \
	 $(SRCD)E16ANA_GTRResponse.cc \
	 $(SRCD)E16ANA_GTRAnalyzer2.cc \
	 $(SRCD)E16ANA_TrackFinding_Nakai.cc
#$(SRCED)g4out2tree.cc


OBJS0 = $(SRCS0:$(SRCD)%.cc=$(OBJD)%.o)
DEPENDS=$(SRCS0:$(SRCD)%.cc=$(DEPENDD)%.d)


${LIB_STAT}: ${OBJS0}
	${AR} ${ARFLAGS} $@ $^

${LIB_SO}: ${SRCS0} 
	${CC} ${SHADOP} ${CCFLAGS} ${INCDIR} -o $@ $^

#exe:E16G4-yok.stat
exe:E16G4.stat
#exe:stat
#exe:g4out2tree.stat

stat:${TARGET1}.stat
dyn:${TARGET1}.dyn
vis-stat:${TARGET1}.vis-stat
#g4out2tree:${TARGET2}.stat
g4out2tree:${TARGET2}
#g4out2tree:${TARGET2}.stat
stat2:${EXAMPLE3}.stat


$(OBJD)%.o: $(SRCED)%.cc
	${CC} ${CCFLAGS} ${INCDIR} -c  $< -o $@
$(OBJD)%vis.o: $(SRCED)%.cc
	${CC} ${CCFLAGS} ${INCDIR} -c  $< -o $@ -DG4VIS_USE

%.stat: $(OBJED)%.o ${LIB_STAT}
	@echo " --  re-link because $? is/are modified --"
	@echo " --  stat --"
	nice time ${CC} ${STATFLAGS} -o $@ $< ${E16G4LIBlocal}  ${E16ANALIB} ${G4LIB} ${CLHEPLIB}  ${ROOTLIBS_STAT}
#	nice time ${CC} ${STATFLAGS} -o wrk/$@ $< ${E16G4LIBlocal}  ${E16ANALIB} ${G4LIB} ${CLHEPLIB}  ${ROOTLIBS_STAT};	mv wrk/$@ $@

%.stat2: $(OBJED)%.o ${LIB_STAT}
	@echo " --  re-link because $? is/are modified --"
	@echo " --  stat --"
	nice time ${CC} ${STATFLAGS} -o $@ $< ${E16G4LIBlocal}  ${E16ANALIB} ${G4LIB} ${CLHEPLIB}  ${ROOTLIBS_STAT}
#	nice time ${CC} ${STATFLAGS} -o wrk/$@ $< ${E16G4LIBlocal}  ${E16ANALIB} ${G4LIB} ${CLHEPLIB}  ${ROOTLIBS_STAT};	mv wrk/$@ $@


%.dyn: $(OBJED)%.o ${LIB_SO}
	@echo " --  dyn --"
	nice time ${CC}  -o $@ -DG4VIS_USE $< ${E16G4LIBlocal} ${E16ANALIB} ${G4LIB} ${CLHEPLIB}  ${ROOTLIBS_DYN}

%.vis-stat: $(OBJED)%vis.o $(LIB_STAT)
	nice time ${CC} ${STATFLAGS}  -o $@ $<  ${E16G4LIBlocal}  ${E16ANALIB} ${G4LIB} ${CLHEPLIB}  ${ROOTLIBS_STAT};


#%.g4out2tree: $(OBJED)g4out2tree.o ${LIB_STAT}
g4out2tree: $(OBJED)g4out2tree.o ${LIB_STAT}
	@echo " --  re-link because $? is/are modified --"
	@echo " --  stat --"
	nice time ${CC} ${STATFLAGS}  -o $@ $<  ${G4LIB} ${E16G4LIBlocal} ${E16ANALIB} ${CLHEPLIB} ${G4LIB} ${ROOTLIBS_STAT} ${CLHEPLIB}
#	nice time ${CC} ${STATFLAGS}  -o $@ $<  ${E16G4LIBlocal}  ${E16ANALIB} ${G4LIB} ${CLHEPLIB}  ${ROOTLIBS_STAT_G4OUT2TREE}

#$(EXAMPLE3): $(OBJED)$(EXAMPLE3).o ${LIB_STAT}
#	@echo " --  re-link because $? is/are modified --"
#	@echo " --  stat --"
#	nice time ${CC} ${STATFLAGS} -o $@ $< ${E16G4LIBlocal}  ${E16ANALIB} ${G4LIB} ${CLHEPLIB}  ${ROOTLIBS_STAT}



#-------------------------------------------
# tool section


.PHONY: clean

clean:
	rm -rf $(OBJD)*.o $(DEPENDD)*.d core ${TARGETS} 

displaymacro:
	@echo "targ   "   $(TARGETS)
	@echo "OBJS0  "   $(OBJS0) 
	@echo "DEPENDS"   $(DEPENDS)
	@echo "inc"       $(INCDIR)

#-----------------------------------------------------------------
$(OBJD)%.o : $(SRCD)%.cc
#        $(CC) $(CFLAGS) -c -o $@ $<   
	@echo " ---  recompile because *** $? *** is modified ---"
	${CC} ${CCFLAGS} ${INCDIR} -c  $< -o $@ 

$(OBJED)%.o : $(SRCED)%.cc
	@echo " ---  recompile because *** $? *** is modified ---"
	${CC} ${CCFLAGS} ${INCDIR} -c  $< -o $@ 
#-------------------------------------------
#  dependency check section

# substitute '/' in the path to '\/'  to fit as the sed argument
TEMP= $(subst /,\/, $@)
TEMP1= $(basename $@)
TEMP2= $(subst /,\/, $(TEMP1))


$(OBJD)%.d: $(SRCD)%.cc 
#       @echo $@ $(TEMP) $(TEMP1) $(TEMP2)
	@set -e; $(CC) -MM ${INCDIR}  $< | sed 's/\($*\)\.o[ :]*/$(TEMP2).o $(TEMP) : /g' > $@;        [ -s $@ ] || rm -f $@


-include $(DEPENDS)

# '-' suppress the error when the file does not exist
# '@' suppress the echo back of the commandline

#-------------------------




