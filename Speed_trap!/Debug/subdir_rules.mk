################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
AnarenUtils.obj: ../AnarenUtils.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.3.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/workspace/Speed_trap!" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.3.LTS/include" --advice:power=all --define=__MSP430G2553__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="AnarenUtils.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

cc11xL_spi.obj: ../cc11xL_spi.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.3.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/workspace/Speed_trap!" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.3.LTS/include" --advice:power=all --define=__MSP430G2553__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="cc11xL_spi.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

hal_board.obj: ../hal_board.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.3.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/workspace/Speed_trap!" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.3.LTS/include" --advice:power=all --define=__MSP430G2553__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="hal_board.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

hal_digio2.obj: ../hal_digio2.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.3.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/workspace/Speed_trap!" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.3.LTS/include" --advice:power=all --define=__MSP430G2553__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="hal_digio2.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

hal_int.obj: ../hal_int.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.3.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/workspace/Speed_trap!" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.3.LTS/include" --advice:power=all --define=__MSP430G2553__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="hal_int.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

hal_int_rf_msp_exp430g2.obj: ../hal_int_rf_msp_exp430g2.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.3.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/workspace/Speed_trap!" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.3.LTS/include" --advice:power=all --define=__MSP430G2553__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="hal_int_rf_msp_exp430g2.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

hal_msp_exp430g2_spi.obj: ../hal_msp_exp430g2_spi.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.3.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/workspace/Speed_trap!" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.3.LTS/include" --advice:power=all --define=__MSP430G2553__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="hal_msp_exp430g2_spi.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.3.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/workspace/Speed_trap!" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.3.LTS/include" --advice:power=all --define=__MSP430G2553__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="main.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

uart.obj: ../uart.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.3.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/workspace/Speed_trap!" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.3.LTS/include" --advice:power=all --define=__MSP430G2553__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="uart.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

utility.obj: ../utility.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.3.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/workspace/Speed_trap!" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.3.LTS/include" --advice:power=all --define=__MSP430G2553__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="utility.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


