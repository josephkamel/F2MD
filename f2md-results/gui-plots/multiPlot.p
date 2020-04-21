### Start multiplot (3x3 layout)

set multiplot layout 4,3 rowsfirst
#set lmargin 2.5
#set rmargin 2.5
#set tmargin 2.5
#set bmargin 2.5

# --- GRAPH a
load 'gui-plots/plotMdmAll.p'
# --- GRAPH b

# --- GRAPH c
#load 'gui-plots/plotMda-DetectionLatency.p'
load 'gui-plots/plotV1vsV2Faulty.p'
# --- GRAPH d
load 'gui-plots/plotV1vsV2Attack.p'
# --- GRAPH e

load 'gui-plots/plotDensity.p'

load 'gui-plots/plotV1vsV2Recall.p'
# --- GRAPH f
load 'gui-plots/plotV1vsV2Precision.p'
# --- GRAPH g
load 'gui-plots/plotV1vsV2Accuracy.p'
# --- GRAPH h
load 'gui-plots/plotV1vsV2F1Score.p'
# --- GRAPH i
load 'gui-plots/plotV1vsV2Informedness.p'
# --- GRAPH j
load 'gui-plots/plotV1vsV2Markedness.p'
# --- GRAPH k
load 'gui-plots/plotV1vsV2MCC.p'
# --- GRAPH j
load 'gui-plots/plotV1vsV2Kappa.p'


# --- GRAPH k
#load 'gui-plots/plotMda-DetectionRate.p'


unset multiplot
### End multiplot  


