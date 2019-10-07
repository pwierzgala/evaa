library verilog;
use verilog.vl_types.all;
entity ufmclk is
    port(
        oscena          : in     vl_logic;
        osc             : out    vl_logic
    );
end ufmclk;
