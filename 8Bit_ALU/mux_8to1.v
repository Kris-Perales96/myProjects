module mux_8to1(mux8Out, sel, sig1, sig2, sig3, sig4, sig5, sig6, sig7, sig8);

reg muxOut, muxOut1;
input wire sig1, sig2, sig3, sig4, sig5, sig6, sig7, sig8;
input wire [2:0] sel;
output mux8Out;

MUX_4_to_1 m1(muxOut, sel[1:0], sig1, sig2, sig3, sig4);
MUX_4_to_1 m2(muxOut1, sel[1:0], sig5, sig6, sig7, sig8);
wire selNot, w, w1;

not(selNot, sel[2]);

and and0(w, mux1, selNot);
and and1(w1, mux2, sel[2]);

or or0(mux8Out, w, w1);

endmodule
