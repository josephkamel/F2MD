### Start multiplot (3x3 layout)

set multiplot layout 4,3 rowsfirst
#set lmargin 2.5
#set rmargin 2.5
#set tmargin 2.5
#set bmargin 2.5

# --- GRAPH a
load 'plots/plotMdmAll.p'
# --- GRAPH b

# --- GRAPH c
#load 'plots/plotMda-DetectionLatency.p'
load 'plots/plotV1vsV2Faulty.p'
# --- GRAPH d
load 'plots/plotV1vsV2Attack.p'
# --- GRAPH e

load 'plots/plotDensity.p'

load 'plots/plotV1vsV2Recall.p'
# --- GRAPH f
load 'plots/plotV1vsV2Precision.p'
# --- GRAPH g
load 'plots/plotV1vsV2Accuracy.p'
# --- GRAPH h
load 'plots/plotV1vsV2F1Score.p'
# --- GRAPH i
load 'plots/plotV1vsV2Informedness.p'
# --- GRAPH j
load 'plots/plotV1vsV2Markedness.p'
# --- GRAPH k
load 'plots/plotV1vsV2MCC.p'
# --- GRAPH j
load 'plots/plotV1vsV2Kappa.p'


# --- GRAPH k
#load 'plots/plotMda-DetectionRate.p'


unset multiplot
### End multiplot  


