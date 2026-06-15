#makefile for E16G4 by S. Yokkaichi
#    Last modified at <2017-09-06 17:37:19 >
#memo
# setenv E16SYS /ccj/u/E16; source $E16SYS/bin/E16setup.csh

#--------------------------------
# directory layout section
SRCD    = src/
OBJD    = obj/
DEPENDD = obj/
SRCED    = examples/
OBJED    = obj/

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

# --- ROOT Detection ---
# 1. Try root-config from PATH
# 2. Try $(ROOTSYS)/bin/root-config
# 3. Try $(E16SYS)/bin/root-config (CCJ fallback)
ROOT_CONFIG := $(shell which root-config 2>/dev/null)
ifeq ($(ROOT_CONFIG),)
  ifneq ($(ROOTSYS),)
    ROOT_CONFIG := $(ROOTSYS)/bin/root-config
  endif
endif
ifeq ($(wildcard $(ROOT_CONFIG)),)
  ROOT_CONFIG := $(E16SYS)/bin/root-config
endif

# Extract flags using root-config
ROOT_CFLAGS := $(shell $(ROOT_CONFIG) --cflags 2>/dev/null)
ROOT_LIBS   := $(shell $(ROOT_CONFIG) --libs 2>/dev/null)

# Fallbacks for CCJ (ROOT 5) if root-config detection fails
ROOTLIBS_DYN_CCJ  = -L$(E16SYS)/lib64 -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic -lMinuit -lGenVector -lSpectrum
ROOTLIBS_STAT_CCJ = -L$(E16SYS)/lib64 -lRoot -lpcre -llzma -pthread -lz -lm -ldl

ifeq ($(ROOT_LIBS),)
  ROOTLIBS_DYN  = $(ROOTLIBS_DYN_CCJ)
  ROOTLIBS_STAT = $(ROOTLIBS_STAT_CCJ)
else
  ROOTLIBS_DYN  = $(ROOT_LIBS) -lMinuit -lEG
  ROOTLIBS_STAT = $(ROOT_LIBS) -lMinuit -lEG
endif

ifeq ($(ROOT_CFLAGS),)
  # Fallback include path for CCJ
  ROOTINCLUDE_FLAGS = -I$(E16SYS)/include/root
else
  ROOTINCLUDE_FLAGS = $(ROOT_CFLAGS)
endif
# Clean std flags to prevent overriding modern standards
ROOTINCLUDE_FLAGS := $(filter-out -std=c++11 -std=c++14 -std=c++17 -std=c++20, $(ROOTINCLUDE_FLAGS))
# ----------------------

# --- Geant4 Detection ---
G4_CONFIG := $(shell which geant4-config 2>/dev/null)
ifeq ($(G4_CONFIG),)
  G4INCLUDE_FLAGS = -I$(E16SYS)/include/Geant4
  G4_LIBS         = -L$(E16SYS)/lib64 -lG4visHepRep -lG4vis_management -lG4visXXX -lG4Tree -lG4physicslists -lG4run -lG4tracking -lG4processes -lG4materials -lG4GMocren -lG4modeling -lG4event -lG4digits_hits -lG4geometry  -lG4intercoms -lG4analysis -lG4FR -lG4error_propagation -lG4expat -lG4gl2ps -lG4global -lG4graphics_reps -lG4interfaces -lG4parmodels -lG4persistency -lG4RayTracer -lG4readout -lG4track  -lG4OpenGL  -lG4VRML
else
  G4INCLUDE_FLAGS := $(shell $(G4_CONFIG) --cflags 2>/dev/null)
  G4_LIBS         := $(shell $(G4_CONFIG) --libs 2>/dev/null)
endif
# Clean std flags to prevent overriding modern standards
G4INCLUDE_FLAGS   := $(filter-out -std=c++11 -std=c++14 -std=c++17 -std=c++20, $(G4INCLUDE_FLAGS))

CLHEPLIB =
QTLIBS   = -lQt5Widgets -lQt5Gui -lQt5Core -lQt5OpenGL

G4LIB    = $(G4_LIBS)

3G4LIB    = -L$(E16SYS)/lib64 -lG4visHepRep -lG4vis_management -lG4visXXX -lG4Tree -lG4physicslists -lG4run -lG4tracking -lG4processes -lG4materials -lG4GMocren -lG4modeling -lG4event -lG4digits_hits -lG4geometry  -lG4intercoms -lG4analysis -lG4FR -lG4error_propagation -lG4expat -lG4gl2ps -lG4global -lG4graphics_reps -lG4interfaces -lG4parmodels -lG4persistency -lG4RayTracer -lG4readout -lG4track  -lG4particles  -lG4OpenGL  -lG4VRML -lG4zlib

# --- E16ANA Local Submodule Build ---
E16ANA_LOCAL_DIR = ./E16ANA_src
E16ANA_LOCAL_INC = ./E16ANA_include
E16INCLUDE       = $(E16ANA_LOCAL_INC)
E16ANALIB        = lib64/libE16ANA_local.a
LIB_E16ANA_LOCAL = lib64/libE16ANA_local.a

E16ANA_SRCS = $(wildcard $(E16ANA_LOCAL_DIR)/*.cc)
# Remove the ones we replaced with E88_ versions so they don't compile twice or collide
E16ANA_SRCS := $(filter-out $(E16ANA_LOCAL_DIR)/E16ANA_GeometryV2.cc $(E16ANA_LOCAL_DIR)/E16ANA_G4OutputData.cc $(E16ANA_LOCAL_DIR)/E16ANA_TrackFinding_Nakai.cc, $(E16ANA_SRCS))
E16ANA_OBJS = $(patsubst $(E16ANA_LOCAL_DIR)/%.cc, $(OBJD)e16ana_%.o, $(E16ANA_SRCS))

$(LIB_E16ANA_LOCAL): $(E16ANA_OBJS)
	@mkdir -p lib64
	$(AR) $(ARFLAGS) $@ $^

$(OBJD)e16ana_%.o: $(E16ANA_LOCAL_DIR)/%.cc
	@mkdir -p $(OBJD)
	$(CC) $(CCFLAGS) -fpermissive -include E16ANA_include/E16ANA_namespace_shim.h $(INCDIR) -c $< -o $@
# ------------------------------------


E16G4LIBlocal   = lib64/libE16G4.a

CC = g++
AR = ar
ARFLAGS = rv
SHADOP = -shared -fPIC

INCDIR= -I./include/ -I$(E16INCLUDE) -I./E16G4_include $(ROOTINCLUDE_FLAGS) $(G4INCLUDE_FLAGS)

CCFLAGS = -O3 -Wall -g -std=c++17 -fPIC
STATFLAGS = ${CCFLAGS} 


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
	${LIB_E16ANA_LOCAL} \
	${TARGET1}.stat ${TARGET1}.dyn ${TARGET1}.vis-stat \
	${EXAMPLE1}  ${TARGET2}  ${EXAMPLE3}.stat

all: ${TARGETS}
lib: ${LIB_STAT} 

E16G4_LOCAL_DIR = ./E16G4_src

SRCS_E16G4 = $(wildcard $(E16G4_LOCAL_DIR)/*.cc)
SRCS_E16G4 := $(filter-out $(E16G4_LOCAL_DIR)/E16G4_Analysis2.cc $(E16G4_LOCAL_DIR)/E16G4_PrimaryGeneratorAction2.cc $(E16G4_LOCAL_DIR)/E16G4_PrimaryGeneratorAction3.cc $(E16G4_LOCAL_DIR)/E16G4_PhysicsList.cc $(E16G4_LOCAL_DIR)/E16G4_PhysicsListHP.cc $(E16G4_LOCAL_DIR)/E16G4_Transportation.cc $(E16G4_LOCAL_DIR)/E16G4_VD.cc $(E16G4_LOCAL_DIR)/E16G4_VDCylinder.cc, $(SRCS_E16G4))
SRCS_E88   = $(SRCD)E88G4_RPC.cc \
             $(SRCD)E88G4_RPCHit.cc \
             $(SRCD)E88G4_RPCSD.cc \
             $(SRCD)E88G4_DetectorConstruction.cc \
             $(SRCD)E88_PhysicsList.cc \
             $(SRCD)E88_GeometryV2.cc \
             $(SRCD)E88_G4OutputData.cc \
             $(SRCD)E88_TrackFinding_Nakai.cc \
             $(SRCD)E88G4_VD.cc \
             $(SRCD)E88G4_VDCylinder.cc

SRCS0 = $(SRCS_E16G4) $(SRCS_E88)

OBJS_E16G4 = $(SRCS_E16G4:$(E16G4_LOCAL_DIR)/%.cc=$(OBJD)e16g4_%.o)
OBJS_E88   = $(SRCS_E88:$(SRCD)%.cc=$(OBJD)%.o)

OBJS0 = $(OBJS_E16G4) $(OBJS_E88)
DEPENDS = $(SRCS_E88:$(SRCD)%.cc=$(DEPENDD)%.d) $(SRCS_E16G4:$(E16G4_LOCAL_DIR)/%.cc=$(DEPENDD)e16g4_%.d)


${LIB_STAT}: ${OBJS0}
	${AR} ${ARFLAGS} $@ $^

${LIB_SO}: ${OBJS0} 
	${CC} ${SHADOP} ${CCFLAGS} -o $@ $^

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
	${CC} ${CCFLAGS} -fpermissive -include E16ANA_include/E16ANA_namespace_shim.h ${INCDIR} -c  $< -o $@
$(OBJD)%vis.o: $(SRCED)%.cc
	${CC} ${CCFLAGS} -fpermissive -include E16ANA_include/E16ANA_namespace_shim.h ${INCDIR} -c  $< -o $@ -DG4VIS_USE

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
	nice time ${CC}  -o $@ -DG4VIS_USE $< ${E16G4LIBlocal} ${E16ANALIB} ${G4LIB} ${CLHEPLIB} ${QTLIBS} ${ROOTLIBS_DYN}

%.vis-stat: $(OBJED)%vis.o $(LIB_STAT)
	nice time ${CC} ${STATFLAGS}  -o $@ $<  ${E16G4LIBlocal}  ${E16ANALIB} ${G4LIB} ${CLHEPLIB}  ${QTLIBS} ${ROOTLIBS_STAT};


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
$(OBJD)e16g4_%.o : $(E16G4_LOCAL_DIR)/%.cc
	@echo " ---  recompile because *** $? *** is modified ---"
	$(CC) $(CCFLAGS) -fpermissive -include E16G4_include_shim.h $(INCDIR) -c $< -o $@

$(OBJD)%.o : $(SRCD)%.cc
#        $(CC) $(CFLAGS) -c -o $@ $<   
	@echo " ---  recompile because *** $? *** is modified ---"
	${CC} ${CCFLAGS} ${INCDIR} -c  $< -o $@ 

$(OBJED)%.o : $(SRCED)%.cc
	@echo " ---  recompile because *** $? *** is modified ---"
	${CC} ${CCFLAGS} -fpermissive -include E16ANA_include/E16ANA_namespace_shim.h ${INCDIR} -c  $< -o $@ 
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




