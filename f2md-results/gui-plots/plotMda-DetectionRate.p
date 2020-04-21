set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytics auto                          # set ytics automatically
set y2tics auto 			# set ytics automatically
set title "Absolute Detection Per Attacker"
set xlabel "Time (Min)"
set ylabel "Detection Rate (%)"
set grid
show grid 
funcT(x) = x / 60
func3(x,y) = y<=0  ? 0 : 100*x / y

stats "mdaV1.dat" using (lastV1=func3($6,$7)) nooutput
stats "mdaV2.dat" using (lastV2=func3($6,$7)) nooutput

stats "mdaV1.dat" using (totV1=$7) nooutput
stats "mdaV2.dat" using (totV2=$7) nooutput

stats "mdaV1.dat" using (lastV1F=func3($3,$4)) nooutput
stats "mdaV2.dat" using (lastV2F=func3($3,$4)) nooutput

stats "mdaV1.dat" using (totV1F=$4) nooutput
stats "mdaV2.dat" using (totV2F=$4) nooutput


set label sprintf("V1 = %3.5g\%% (%3.5g) \nV2 = %3.5g\%% (%3.5g) \nV1F = %3.5g\%% (%3.5g) \nV2F = %3.5g\%% (%3.5g)" ,lastV1, totV1, lastV2, totV2, lastV1F, totV1F, lastV2F, totV2F) at graph 0.02,0.16 tc rgb '#00bb00'

plot "mdaV2.dat" using (funcT($2)):(func3($6,$7))  title 'V2 Rate' with linespoints linestyle 2 lw 2 pi 30 ps 0.75 lc rgb "blue",\
"mdaV1.dat" using (funcT($2)):(func3($6,$7))  title 'V1 Rate' with lines lw 2 lt rgb "red"

#"mdaV1.dat" using (funcT($2)):(func3($3,$4)) title 'V1 False' with lines lc rgb "red" axes x1y2,\
#"mdaV2.dat" using (funcT($2)):(func3($3,$4))  title 'V2 False' with lines lc rgb "blue" axes x1y2,\
