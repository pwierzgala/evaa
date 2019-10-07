library verilog;
use verilog.vl_types.all;
entity divider1 is
    port(
        osc             : in     vl_logic;
        clk             : out    vl_logic;
        clk1            : out    vl_logic
    );
end divider1;
