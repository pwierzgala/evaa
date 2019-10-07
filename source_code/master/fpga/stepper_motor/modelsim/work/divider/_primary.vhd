library verilog;
use verilog.vl_types.all;
entity divider is
    port(
        osc             : in     vl_logic;
        clk             : out    vl_logic
    );
end divider;
