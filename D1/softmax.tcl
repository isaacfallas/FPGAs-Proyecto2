#
# Softmax Accelerator - Design 1
# FPGAs Project 2
#

# Create project
open_project softmax_d1
set_top softmax

# Add source files
add_files "softmax.cpp"
add_files -tb "softmax_tb.cc"

# Open solution with Vitis flow
open_solution -flow_target vitis solution

# Set target device: AMD Kria KV260
set_part {xck26-sfvc784-2LV-c}

# Clock: 250 MHz
create_clock -period 250MHz -name default

# Interface configuration
config_dataflow -strict_mode warning
config_rtl -deadlock_detection sim
config_interface -m_axi_conservative_mode=1
config_interface -m_axi_addr64

# Run synthesis
csynth_design
close_project
puts "HLS Design 1 (No optimizations) completed successfully"
exit
