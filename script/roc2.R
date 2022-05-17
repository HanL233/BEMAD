library('pROC')
dat <- read.table('score.txt',header=T)
dat2 <- read.table('score3.txt',header=T)
colnames(dat) <- c("type","sample","score")
A <- array();i <- 1;for(n in seq(nrow(dat))){if(n>1 && dat[n-1,1]=="B" && dat[n,1]=="A"){i <- i+1};A <- c(A,i)};A <- A[-1]
C <- array();for(n in seq(10)){B <- roc(dat$type[A==n],as.numeric(scale(dat$score[A==n])+scale(dat2$score[A==n])));dat$score[A==n] <- (as.numeric(scale(dat$score[A==n])+scale(dat2$score[A==n])));C <- c(C,B$auc);dat$score[A==n]<-as.numeric(scale(dat$score[A==n]))};C <- C[-1]
print(roc(dat$type,dat$score))
print(C)
print(summary(C))
print(t.test(C))

write.table(dat,file='a')
