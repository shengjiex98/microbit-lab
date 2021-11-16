#!/bin/bash
openocd -f interface/cmsis-dap.cfg -f target/nrf52.cfg -c "program build/assembly_lecture_examples.hex verify reset exit"
