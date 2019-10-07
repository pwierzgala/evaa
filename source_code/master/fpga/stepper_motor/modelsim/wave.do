onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -format Literal /test_stepmot/test_vec
add wave -noupdate -format Logic /test_stepmot/oscena
add wave -noupdate -format Literal /test_stepmot/test_phase
add wave -noupdate -format Logic /test_stepmot/osc
add wave -noupdate -format Logic /test_stepmot/clk_t
add wave -noupdate -format Logic /test_stepmot/clk1
add wave -noupdate -format Literal /test_stepmot/t
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {0 ps} 0}
configure wave -namecolwidth 175
configure wave -valuecolwidth 39
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
update
WaveRestoreZoom {593356410565 ps} {1068220037426 ps}
