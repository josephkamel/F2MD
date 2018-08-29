from sklearn.externals import joblib
import numpy as np
import json
import os

# load the classifier from file
os.chdir("/home/joseph/Projects/F2MD/VeinsMDV2/omnetpp-workspace/veins-veins-4.6/src/veins/modules/application/misbehaviorDetection/mdChecks/")
clf = joblib.load('mlClassifiers/clf.pkl')

# load test data from file
test_data = []
with open('mlTrainersAndPredictors/test_data_file.json', 'r') as test_data_file:
    tmp = json.load(test_data_file)
    for data in tmp['test_data']:
        test_data.append(data)

numpy_array = np.asarray(test_data)

# predict using saved classifier
predictions = np.array(clf.predict(numpy_array), dtype=np.float64)
print "Predictions:"
print predictions

# build output json string
output = "{prediction:["
p = ""
for index, prediction in enumerate(predictions):
    p = p + prediction.__repr__()
    if not index is len(predictions)-1:
        p = p + ","
output = output + p + "]}"

with open('mlResults/pred_file.json', 'w') as fp:
    json.dump(output, fp)
