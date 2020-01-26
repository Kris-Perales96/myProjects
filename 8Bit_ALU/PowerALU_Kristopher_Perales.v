module PowerALU_Kristopher_Perales(aluOut, aluCout, aluOF, enable, opcode, a, b, clk, reset);
output [7:0] aluOut;
output aluCout, aluOF;
input reset;
input clk, enable;
input [3:0] opcode;
input [7:0] a, b;
wire [7:0] A, B;
wire muxCarry, muxOF;
wire [7:0] aTransfer;  
wire [7:0] opOut, NANDout, ANDout, ORout, NORout, XORout, XNORout;
wire  LT, GT, EQ, LT1, GT1, EQ1, LT2,GT2,EQ2,cout, cout1, cout2,cout3,cout4,cout5;
wire carry_q, OF_q, carryD, OFD;
wire [7:0] sum, sum_q, sumD;
wire zout1;
reg def = 1'b0;

DFF8Project A_q(A, clk, reset, a); 
DFF8Project B_q(B, clk, reset, b); 

fourcompare four1(L_LT, L_GT, A[3:0], B[3:0]);
fourcompare four2(M_LT, M_GT, A[7:4], B[7:4]);

passSignal transfer(A, aTransfer);
RCA8Project rca00(A,8'd1, 1'd0, cout1, opOut1, OF1);
RCA8Project rca01(A, B, 1'b0, cout2, opOut2, OF2);
RCA8Project rca02(A, B, 1'b1, cout3, opOut3, OF3);
RCA8Project rca03(A, 8'd1, 1'b1, cout4, opOut4, OF4); 
RCA8Project rca04(B,8'd1, 1'd0, cout5, opOut5, OF5);
eightComplement eightComplement1(A,8'd1,opOut); 
eightNAND eightNAND1(A,B,NANDout); 
eightAND eightAND1(A,B, ANDout); 
eightOR eightOR1(A,B,ORout);
eightNOR eightNOR1(A,B,NORout); 
eightXOR eightXOR1(A,B,XORout);
eightXNOR eightXNOR1(A,B,XNORout);
eightCompare greater(LT, GT, EQ, M_GT, M_LT, L_GT, L_LT, enable); 
eightCompare less(LT1, GT1, EQ1, M_GT, M_LT, L_GT, L_LT, enable);
eightCompare equal(LT2, GT2, EQ2, M_GT, M_LT, L_GT, L_LT, enable);

MUXeight sum1(sum, opcode, aTransfer, opOut1, opOut2, opOut3,opOut4,opOut5, XORout, NANDout, ANDout, ORout, NORout, XORout, XNORout, GT, LT1, EQ2, opOut);
mux_16to1 carryOut(muxCarry, opcode, def, cout, cout1, cout2,cout3,cout4,cout5, def, def,def,def,def,def,def,def,def,def,def);
mux_16to1 OFOut(muxOF, opcode, def, OF1, OF2, OF3,OF4,OF5, def, def,def,def,def,def,def,def,def,def,def);

DFF8Project sumMuxOut(sum_q, clk, reset, sum);
DFF1Project  carryMuxOut(carry_q, clk, reset, muxCarry);
DFF1Project  OFMuxOut(OF_q, clk, reset, muxOF);

AccumulatorEight sumacc(sumD, sum_q, reset, clk);
AccumulatorOne sumacc1(carryD, carry_q, reset, clk);
AccumulatorOne sumacc2(OFD, OF_q, reset, clk);

DFF8Project aluSumOut(aluSum, clk, reset, sumD);
DFF1Project  aluCarryOut(aluCout, clk, reset, carryD);
DFF1Project  aluOFAccOut(aluOF, clk, reset, OFD);

endmodule
