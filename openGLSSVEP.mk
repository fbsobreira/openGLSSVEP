##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=openGLSSVEP
ConfigurationName      :=Debug
WorkspacePath          := "/home/fbsobreira/src/openGLSSVEP"
ProjectPath            := "/home/fbsobreira/src/openGLSSVEP"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=
Date                   :=10/22/15
CodeLitePath           :="/home/fbsobreira/.codelite"
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="openGLSSVEP.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g $(Preprocessors)
CFLAGS   :=  -g $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/glhelper.cpp$(ObjectSuffix) $(IntermediateDirectory)/wglext.cpp$(ObjectSuffix) $(IntermediateDirectory)/ssvepobject.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(SharedObjectLinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)
	@$(MakeDirCommand) "/home/fbsobreira/src/openGLSSVEP/.build-debug"
	@echo rebuilt > "/home/fbsobreira/src/openGLSSVEP/.build-debug/openGLSSVEP"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/fbsobreira/src/openGLSSVEP/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM "main.cpp"

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) "main.cpp"

$(IntermediateDirectory)/glhelper.cpp$(ObjectSuffix): glhelper.cpp $(IntermediateDirectory)/glhelper.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/fbsobreira/src/openGLSSVEP/glhelper.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/glhelper.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/glhelper.cpp$(DependSuffix): glhelper.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/glhelper.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/glhelper.cpp$(DependSuffix) -MM "glhelper.cpp"

$(IntermediateDirectory)/glhelper.cpp$(PreprocessSuffix): glhelper.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/glhelper.cpp$(PreprocessSuffix) "glhelper.cpp"

$(IntermediateDirectory)/wglext.cpp$(ObjectSuffix): wglext.cpp $(IntermediateDirectory)/wglext.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/fbsobreira/src/openGLSSVEP/wglext.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wglext.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wglext.cpp$(DependSuffix): wglext.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/wglext.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/wglext.cpp$(DependSuffix) -MM "wglext.cpp"

$(IntermediateDirectory)/wglext.cpp$(PreprocessSuffix): wglext.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wglext.cpp$(PreprocessSuffix) "wglext.cpp"

$(IntermediateDirectory)/ssvepobject.cpp$(ObjectSuffix): ssvepobject.cpp $(IntermediateDirectory)/ssvepobject.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/fbsobreira/src/openGLSSVEP/ssvepobject.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ssvepobject.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ssvepobject.cpp$(DependSuffix): ssvepobject.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ssvepobject.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ssvepobject.cpp$(DependSuffix) -MM "ssvepobject.cpp"

$(IntermediateDirectory)/ssvepobject.cpp$(PreprocessSuffix): ssvepobject.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ssvepobject.cpp$(PreprocessSuffix) "ssvepobject.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


