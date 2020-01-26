module eightCompare(l, t, e, EQcheck, M_GT, M_LT, L_GT, L_LT, enable);
output l, t, e;
input  M_GT, M_LT, L_GT, L_LT;
input  enable;
wire w1, w2, w3, w4, w5, w6;
output EQcheck;

and a1(w1, L_GT, ~M_GT);
and a2(w2, ~M_LT, L_LT);
and a3(EQcheck, enable, w5, w6);

or o1(w3, w1, M_GT);
or o2(w4, w2, M_LT);

nand n1(w5, enable, w3);
nand n2(w6, enable, w4);

not not1(l, w5);
not not2(t, w6);
nor nor1(e, w5, w6); 

endmodule
