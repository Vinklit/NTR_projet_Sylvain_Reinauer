################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
audioProcessing.obj: ../audioProcessing.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/bin/cl6x" -mv64+ -g -O2 --include_path="C:/ti/ccsv5/tools/compiler/c6000_7.4.4/include" --include_path="C:/Users/Sylvain/Desktop/Cours/4_Annee/NTR/Workspace/01-LABOS/02-Labs(Sotfware)/NTR_projet_Sylvain_Reinauer/Release" --include_path="C:/ti/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --diag_warning=225 --abi=coffabi --preproc_with_compile --preproc_dependency="audioProcessing.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

audio_params.obj: ../audio_params.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/bin/cl6x" -mv64+ -g -O2 --include_path="C:/ti/ccsv5/tools/compiler/c6000_7.4.4/include" --include_path="C:/Users/Sylvain/Desktop/Cours/4_Annee/NTR/Workspace/01-LABOS/02-Labs(Sotfware)/NTR_projet_Sylvain_Reinauer/Release" --include_path="C:/ti/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --diag_warning=225 --abi=coffabi --preproc_with_compile --preproc_dependency="audio_params.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

evmAudioTemplate.obj: ../evmAudioTemplate.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/bin/cl6x" -mv64+ -g -O2 --include_path="C:/ti/ccsv5/tools/compiler/c6000_7.4.4/include" --include_path="C:/Users/Sylvain/Desktop/Cours/4_Annee/NTR/Workspace/01-LABOS/02-Labs(Sotfware)/NTR_projet_Sylvain_Reinauer/Release" --include_path="C:/ti/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --diag_warning=225 --abi=coffabi --preproc_with_compile --preproc_dependency="evmAudioTemplate.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

evmAudioTemplatecfg.cmd: ../evmAudioTemplate.tcf
	@echo 'Building file: $<'
	@echo 'Invoking: TConf Script Compiler'
	"C:/ti/bios_5_42_01_09/xdctools/tconf" -b -Dconfig.importPath="C:/ti/bios_5_42_01_09/packages;" "$<"
	@echo 'Finished building: $<'
	@echo ' '

evmAudioTemplatecfg.s??: | evmAudioTemplatecfg.cmd
evmAudioTemplatecfg_c.c: | evmAudioTemplatecfg.cmd
evmAudioTemplatecfg.h: | evmAudioTemplatecfg.cmd
evmAudioTemplatecfg.h??: | evmAudioTemplatecfg.cmd
evmAudioTemplate.cdb: | evmAudioTemplatecfg.cmd

evmAudioTemplatecfg.obj: ./evmAudioTemplatecfg.s?? $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/bin/cl6x" -mv64+ -g -O2 --include_path="C:/ti/ccsv5/tools/compiler/c6000_7.4.4/include" --include_path="C:/Users/Sylvain/Desktop/Cours/4_Annee/NTR/Workspace/01-LABOS/02-Labs(Sotfware)/NTR_projet_Sylvain_Reinauer/Release" --include_path="C:/ti/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --diag_warning=225 --abi=coffabi --preproc_with_compile --preproc_dependency="evmAudioTemplatecfg.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

evmAudioTemplatecfg_c.obj: ./evmAudioTemplatecfg_c.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/bin/cl6x" -mv64+ -g -O2 --include_path="C:/ti/ccsv5/tools/compiler/c6000_7.4.4/include" --include_path="C:/Users/Sylvain/Desktop/Cours/4_Annee/NTR/Workspace/01-LABOS/02-Labs(Sotfware)/NTR_projet_Sylvain_Reinauer/Release" --include_path="C:/ti/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --diag_warning=225 --abi=coffabi --preproc_with_compile --preproc_dependency="evmAudioTemplatecfg_c.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

i2c_params.obj: ../i2c_params.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/bin/cl6x" -mv64+ -g -O2 --include_path="C:/ti/ccsv5/tools/compiler/c6000_7.4.4/include" --include_path="C:/Users/Sylvain/Desktop/Cours/4_Annee/NTR/Workspace/01-LABOS/02-Labs(Sotfware)/NTR_projet_Sylvain_Reinauer/Release" --include_path="C:/ti/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --diag_warning=225 --abi=coffabi --preproc_with_compile --preproc_dependency="i2c_params.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


