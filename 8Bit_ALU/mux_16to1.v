module mux_16to1(mux16Out, sel, sig1, sig2, sig3, sig4, sig5, sig6, sig7, sig8, sig9, sig10, sig11, sig12, sig13, sig14, sig16);
input wire sig1, sig2, sig3, sig4, sig5, sig6, sig7, sig8, sig9, sig10, sig11, sig12, sig13, sig14, sig16;
output mux16Out;
input wire [3:0] sel;
wire selNot, w, w1;
reg mux8, mux8_1;

mux_8to1 m8_1(mux8, sel[2:0], sig1, sig2, sig3, sig4, sig5, sig6, sig7, sig8);
mux_8to1 m8_2(mux8_1, sel[2:0], sig9, sig10, sig11, sig12, sig13, sig14, sig15, sig16);

not(selNot, sel[3]);

and and0(w, mux8, selNot);
and and1(w1, mux8_1, sel[3]);

or or0(mux16Out, w, w1);

endmodule
