##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=kv2clientApplication
ConfigurationName      :=Debug
WorkspacePath          := "/home/demo/projects/kv2streamer/sample-client-applications/kv2client_codelite"
ProjectPath            := "/home/demo/projects/kv2streamer/sample-client-applications/kv2client_codelite/kv2clientApplication"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=demo
Date                   :=04/01/15
CodeLitePath           :="/home/demo/.codelite"
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="kv2clientApplication.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)/usr/local/include/boost $(IncludeSwitch)/usr/local/include/ $(IncludeSwitch)/usr/local/include/gstreamer-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)kv2client $(LibrarySwitch)gst-wrapper $(LibrarySwitch)oscpack-wrapper $(LibrarySwitch)oscpack $(LibrarySwitch)boost_system $(LibrarySwitch)boost_thread $(LibrarySwitch)glfw $(LibrarySwitch)GL $(LibrarySwitch)gstreamer-1.0 $(LibrarySwitch)gstapp-1.0 $(LibrarySwitch)glib-2.0 $(LibrarySwitch)gmodule-2.0 $(LibrarySwitch)gthread-2.0 $(LibrarySwitch)gobject-2.0 
ArLibs                 :=  "libkv2client" "libgst-wrapper" "liboscpack-wrapper" "liboscpack" "libboost_system" "libboost_thread" "libglfw" "libGL" "libgstreamer-1.0" "libgstapp-1.0" "libglib-2.0" "libgmodule-2.0" "libgthread-2.0" "libgobject-2.0" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)/usr/local/lib/ $(LibraryPathSwitch)/usr/local/lib/gstreamer-1.0 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++ 
CC       := /usr/bin/gcc 
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/GLFWBaseApp.cpp$(ObjectSuffix) $(IntermediateDirectory)/KV2ClientExample.cpp$(ObjectSuffix) $(IntermediateDirectory)/main.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

PostBuild:
	@echo Executing Post Build commands ...
	
	@echo Done

$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:
	@echo Executing Pre Build commands ...
	
	@echo Done


##
## Objects
##
$(IntermediateDirectory)/GLFWBaseApp.cpp$(ObjectSuffix): GLFWBaseApp.cpp $(IntermediateDirectory)/GLFWBaseApp.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/demo/projects/kv2streamer/sample-client-applications/kv2client_codelite/kv2clientApplication/GLFWBaseApp.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GLFWBaseApp.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GLFWBaseApp.cpp$(DependSuffix): GLFWBaseApp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GLFWBaseApp.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GLFWBaseApp.cpp$(DependSuffix) -MM "GLFWBaseApp.cpp"

$(IntermediateDirectory)/GLFWBaseApp.cpp$(PreprocessSuffix): GLFWBaseApp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GLFWBaseApp.cpp$(PreprocessSuffix) "GLFWBaseApp.cpp"

$(IntermediateDirectory)/KV2ClientExample.cpp$(ObjectSuffix): KV2ClientExample.cpp $(IntermediateDirectory)/KV2ClientExample.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/demo/projects/kv2streamer/sample-client-applications/kv2client_codelite/kv2clientApplication/KV2ClientExample.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/KV2ClientExample.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/KV2ClientExample.cpp$(DependSuffix): KV2ClientExample.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/KV2ClientExample.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/KV2ClientExample.cpp$(DependSuffix) -MM "KV2ClientExample.cpp"

$(IntermediateDirectory)/KV2ClientExample.cpp$(PreprocessSuffix): KV2ClientExample.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/KV2ClientExample.cpp$(PreprocessSuffix) "KV2ClientExample.cpp"

$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/demo/projects/kv2streamer/sample-client-applications/kv2client_codelite/kv2clientApplication/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM "main.cpp"

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) "main.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


