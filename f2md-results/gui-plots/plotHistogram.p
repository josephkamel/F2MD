set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels

set style line 2 lc rgb 'black' lt 1 lw 1
set style data histogram
set style histogram cluster gap 1
set style fill pattern border -1

set key box
unset key

set xtic auto                          # set xtics automatically
set ytic auto 			# set ytics automatically
set title "On board Report Accuracy"
set xlabel "Threshold"
set ylabel "Ratio"
set y2label
set grid 
show grid 


#sensitivity TPR=TP/(TP+FN)
TPR(x,y) = x/(x+y)

#precision PPV=TP/(TP+FP)
PPV(x,y) = x/(x+y)

#F1 score F1=2 TP/(2 TP+ FP+ FN)
F(x,y,z) = 2*x/(2*x+y+z)

#F1 score 
FB(x,y) = 2*(x*y)/(x+y)

#accuracy ACC=(TP+TN)/(TP+FP+FN+TN)
ACC(a,b,c,d) = (a+b)/(b+b+c+d)


plot "VarThreAppV1.dat" using (TPR($2,$5)):xticlabel(1) title "SOTA Sensitivity",\
"VarThreAppV2.dat" using (TPR($2,$5)):xticlabel(1) title "SCA Lib Sensitivity",\
"VarThreAppV1.dat" using (PPV($2,$3)):xticlabel(1) title "SOTA Precision",\
"VarThreAppV2.dat" using (PPV($2,$3)):xticlabel(1) title "SCA Lib Precision",\
"VarThreAppV1.dat" using (F($2,$3,$5)):xticlabel(1) title "SCA Lib F-score" with linespoints,\
"VarThreAppV2.dat" using (F($2,$3,$5)):xticlabel(1) title "SCA Lib F-score" with linespoints


#"V1app.dat" using (FB((TPR($2,$5)),(PPV($2,$3)))):xticlabel(1) title "F-score V1" with linespoints,\
#"V2app.dat" using (FB((TPR($2,$5)),(PPV($2,$3)))):xticlabel(1) title "F-score V2" with linespoints


#"V1app.dat" using (ACC($2,$4,$3,$5)):xticlabel(1) title "Accuracy V1" with linespoints,\
#"V2app.dat" using (ACC($2,$4,$3,$5)):xticlabel(1) title "Accuracy V2" with linespoints
