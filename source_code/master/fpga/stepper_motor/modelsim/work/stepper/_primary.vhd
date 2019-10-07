library verilog;
use verilog.vl_types.all;
entity stepper is
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        dir             : in     vl_logic;
        mode            : in     vl_logic;
        move            : in     vl_logic;
        phaseout        : out    vl_logic_vector(3 downto 0)
    );
end stepper;
