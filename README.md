# BEMAD
A tool to predict whether a pulmonary nodule is benign or malignant according to blood cfDNA data.

## [STEP 0] compile the tool and prepare data

```
compile the tool
cd script
gcc -o pond_scale pond_scale.c -lm
cd ..
```
```
decompress the data
cd demo_data
tar -zxvf data.tar.gz
cd ..
```

## [STEP 1] training

```
./script/pond_scale --input1 ./demo_data/data.txt.Cancer.train --input2 ./demo_data/data.txt.Benign.train --width 5 > ./demo_data/pond.txt
```

## [STEP 2] predicting

```
perl ./script/getScore.-1.pl ./demo_data/data.txt.Cancer.train ./demo_data/pond.txt 10 > ./demo_data/score.Cancer.train.-1.txt
perl ./script/getScore.-1.pl ./demo_data/data.txt.Benign.train ./demo_data/pond.txt 10 > ./demo_data/score.Benign.train.-1.txt
perl ./script/getScore.-1.pl ./demo_data/data.txt.Cancer.test ./demo_data/pond.txt 10 > ./demo_data/score.Cancer.test.-1.txt
perl ./script/getScore.-1.pl ./demo_data/data.txt.Benign.test ./demo_data/pond.txt 10 > ./demo_data/score.Benign.test.-1.txt
```
```
perl ./script/getScore.pl ./demo_data/data.txt.Cancer.train ./demo_data/pond.txt 10 > ./demo_data/score.Cancer.train.txt
perl ./script/getScore.pl ./demo_data/data.txt.Benign.train ./demo_data/pond.txt 10 > ./demo_data/score.Benign.train.txt
perl ./script/getScore.pl ./demo_data/data.txt.Cancer.test ./demo_data/pond.txt 10 > ./demo_data/score.Cancer.test.txt
perl ./script/getScore.pl ./demo_data/data.txt.Benign.test ./demo_data/pond.txt 10 > ./demo_data/score.Benign.test.txt
```
```
perl ./script/scale.pl ./demo_data/score.Benign.train.txt ./demo_data/score.Cancer.train.txt ./demo_data/score.Benign.train.-1.txt ./demo_data/score.Cancer.train.-1.txt ./demo_data/score.Benign.test.txt ./demo_data/score.Cancer.test.txt ./demo_data/score.Benign.test.-1.txt ./demo_data/score.Cancer.test.-1.txt 0 > scaled.score.txt
```




