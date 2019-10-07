/*********************************************************************************************************************
* Stepper motor Controller
* December 2006
*********************************************************************************************************************/ 

//TOP LEVEL MODULE

module stepmot(reset,dir,mode,move,phaseout);      //module stepmot is the top level module. 
   		input reset,mode,dir,move;                   //reset when asserted brings the shaft to reference position.
   		output [3:0] phaseout;                       //mode specifies step operation or continuous operation.
   		wire osc,clk;                                //dir is for clockwise or anti clockwise motion.
   		wire oscena;                                 //move shifts the shaft by half step in step mode.   
   		assign oscena=1'b1;
  
 /* Instantiation of ufmclk module. */ 		
 ufmclk  ufmclk1(                                       
        .oscena (oscena),
        .osc  (osc));
                     
 /* Instantiation  of clk divider module.*/
 divider d1( 
        .osc  (osc),                               
        .clk  (clk));                                                     
 
 /* Instantiation  of controller module.*/
 stepper s1(
        .clk (clk),
        .reset (reset),                            
        .dir (dir),
        .mode (mode),
        .move (move),
        .phaseout (phaseout));
endmodule
          
/* Module stepper is the motor controller module */
module stepper (clk,reset,dir,mode,move,phaseout); 
    	input clk,reset,dir,mode;
    	input move;
    	output [3:0] phaseout;
    	reg [3:0] phaseout,position;
    
       
  always@(posedge clk or negedge reset) begin
  if(reset==0) begin                        // motor resets to 0111 at reset=0
     phaseout=4'b0111; 
  end else begin   
      
      /* Mode=0 for continous motion */
      if (mode==0) begin                    
                                            //dir=0 for anticlockwise motion
          case({phaseout})                  //dir=1 for clockwise motion
              4'b0111 :  if (dir==0) 
                         phaseout=4'b0110;
                         else
                         phaseout=4'b0011;
              4'b0011 :  if(dir==0) 
                         phaseout=4'b0111;
                         else
                         phaseout=4'b1011;
              4'b1011 :  if (dir==0) 
                         phaseout=4'b0011;
                         else    
                         phaseout=4'b1001;
              4'b1001 :  if (dir==0) 
                         phaseout=4'b1011;
                         else    
                         phaseout=4'b1101;            
              4'b1101 :  if (dir==0) 
                         phaseout=4'b1001;
                         else    
                         phaseout=4'b1100;
                      
              4'b1100 :  if (dir==0)  
                         phaseout=4'b1101;
                         else 
                         phaseout=4'b1110;  
                      
              4'b1110 :  if (dir==0)
                         phaseout=4'b1100;
                         else    
                         phaseout=4'b0110; 
                       
              4'b0110:   if (dir==0) 
                         phaseout=4'b1110;
                         else    
                         phaseout=4'b0111; 
              default: $display("inavlid input");
          endcase
    
         /* Mode=1 for step motion */
          end else begin
            if(move==1'b0) begin
                                        
            case ({position}) 
            
              4'b0111 :  if (dir==0)                
                         phaseout=4'b0110;
                         else
                         phaseout=4'b0011;
              4'b0011 :  if(dir==0) 
                         phaseout=4'b0111;
                         else
                         phaseout=4'b1011;
              4'b1011 :  if (dir==0) 
                         phaseout=4'b0011;
                         else    
                         phaseout=4'b1001;
              4'b1001 :  if (dir==0) 
                         phaseout=4'b1011;
                         else    
                         phaseout=4'b1101;            
              4'b1101 :  if (dir==0) 
                         phaseout=4'b1001;
                         else    
                         phaseout=4'b1100;                
              4'b1100 :  if (dir==0)  
                         phaseout=4'b1101;
                         else    
                         phaseout=4'b1110;                        
              4'b1110 :  if (dir==0)
                         phaseout=4'b1100;
                         else    
                         phaseout=4'b0110;                       
              4'b0110 :  if (dir==0) 
                         phaseout=4'b1110;
                         else    
                         phaseout=4'b0111; 
              default: $display("inavlid input");
            endcase
         end  
      end
   end
 end
    
/* Motor moves half step on the negetive edge of move*/  
  always@(negedge move or negedge reset) begin
      if (reset==0)
          position=4'b0000;
      else if(reset==1)
          position=phaseout;           
      end
  endmodule                                    

/******************************************************************************************* 
   Module ufmclk is the module 
   which gets the clock from the oscillator 
 ******************************************************************************************/

`timescale 1 ps / 1 ps                           
//synopsys translate_on
module  ufmclk_altufm_osc_7p3
	( 
	osc,
	oscena) /* synthesis synthesis_clearbox=1 */;
	output   osc;
	input   oscena;

	wire  wire_maxii_ufm_block1_osc;

	maxii_ufm   maxii_ufm_block1
	( 
	.arclk(1'b0),
	.ardin(1'b0),
	.arshft(1'b0),
	.bgpbusy(),
	.busy(),
	.drclk(1'b0),
	.drdout(),
	.drshft(1'b0),
	.osc(wire_maxii_ufm_block1_osc),
	.oscena(oscena)
	`ifdef FORMAL_VERIFICATION
	`else
	// synopsys translate_off
	`endif
	,
	.drdin(1'b0),
	.erase(1'b0),
	.program(1'b0)
	`ifdef FORMAL_VERIFICATION
	`else
	// synopsys translate_on
	`endif
	// synopsys translate_off
	,
	.ctrl_bgpbusy(),
	.devclrn(),
	.devpor(),
	.sbdin(),
	.sbdout()
	// synopsys translate_on
	);
	defparam
		maxii_ufm_block1.address_width = 9,
		maxii_ufm_block1.osc_sim_setting = 180000,
		maxii_ufm_block1.lpm_type = "maxii_ufm";
	assign
		osc = wire_maxii_ufm_block1_osc;
endmodule //ufmclk_altufm_osc_7p3
//VALID FILE


// synopsys translate_off
`timescale 1 ps / 1 ps
// synopsys translate_on
module ufmclk (
	oscena,
	osc)/* synthesis synthesis_clearbox = 1 */;

	input	  oscena;
	output	  osc;

	wire  sub_wire0;
	wire  osc = sub_wire0;

	ufmclk_altufm_osc_7p3	ufmclk_altufm_osc_7p3_component (
				.oscena (oscena),
				.osc (sub_wire0));

endmodule


/**************************************************************************************************
* Module divider divides the clock from ufmclk to produce 
* a clock suitable to drive the motor controller 
 *************************************************************************************************/
module divider (osc, clk);                 
	   input osc;
   	   output clk;
   	   reg clk;
       reg [16:0] count;
       initial count=17'b00000000000000000;
 always @( posedge osc)
      begin
         count = count + 1;
         clk = count[16];
      end
endmodule

/***********************************END OF PROGRAM.**********************************************/


