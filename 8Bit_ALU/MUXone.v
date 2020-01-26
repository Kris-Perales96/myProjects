module MUXone(zout, opcode, transfer, rca00, rca01, rca02, rca03, rca04, eightXOR, eightNAND, eightAND, eightOR, eightNOR, eightXOR, eightXNOR, greater, less, equal, eightComplement);
input wire transfer, 
rca00,
rca01,
rca02, 
rca03, 
rca04, 
eightComplement, 
eightNAND, 
eightAND, 
eightOR, 
eightNOR, 
eightXOR, 
eightXNOR,
greater, 
less, 
equal;

input wire [3:0] opcode;
output reg zout;

//Muxone OFOut(muxOF, opcode, def, OF1, OF2, OF3,OF4,OF5, def, def,def,def,def,def,def,def,def,def,def);
//MUXone carryOut(muxCarry, opcode, def, cout, cout1, cout2,cout3,cout4,cout5, def, def,def,def,def,def,def,def,def,def,def);
always @(*)
begin
	case(opcode)
		4'd0:
			zout = transfer;
		4'd1:
		    zout = rca00;

		4'd2:
			zout = rca01;
		4'd3:
			zout = rca02;
			
		4'd4:
			zout = rca03;
			
		4'd5:
			zout = eightComplement;
			
		4'd6:
			zout = eightAND;

			
		4'd7:
			zout = eightNAND;

			
		4'd8:
			zout = eightOR;

			
		4'd9:
			zout = eightNOR;

			
		4'd10:
			zout = eightXOR;

			
		4'd11:
			zout = eightXNOR;

			
		4'd12: 
			zout = greater;
			
			4'd13:
				zout = less;
				
			4'd14:
				zout = equal;
				
			4'd15:
				zout = rca04;
				
	endcase	
	end		
endmodule
