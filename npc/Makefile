all:
	@echo "Write this Makefile by your self."

sim:wave.vcd
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "Write this Makefile by your self."
	gtkwave wave.vcd

Vour.mk:sim_main.cpp our.v
	verilator -Wall --cc --exe --build sim_main.cpp our.v --trace

Vour:Vour.mk
	make -C obj_dir -f Vour.mk Vour

wave.vcd:Vour
	./obj_dir/Vour

include ../Makefile
