library('pROC')
dat <- read.table('score.2w.100.-1.train.txt',header=T)
C <- array()
for(n in unique(dat$round)){
	i <- which(dat$round==n)
	B <- roc(dat$type[i],dat$score[i])
	C <- c(C,B$auc);
	dat$score[i]<-as.numeric(scale(dat$score[i]))
}
C <- C[-1]
print(roc(dat$type,dat$score))
print(C)
print(summary(C))
print(t.test(C))
