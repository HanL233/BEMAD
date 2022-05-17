dat <- read.table('clin.txt')
sco_train <- read.table('score.2w.100.train.txt',header=T,sep="\t")
sco_test <- read.table('score.2w.100.txt',header=T,sep="\t")

dat <- dat[match(sco_test$ID,dat$V6),]
dat$V1 <- sco_test$round
dat$V2 <- sco_test$score1
dat$V3 <- sco_test$score2

for(i in unique(dat$V1)){
	j <- which(sco_train$round==i)
	sd1 <- sd(sco_train$score1[j])
	mean1 <- mean(sco_train$score1[j])
	sd2 <- sd(sco_train$score2[j])
	mean2 <- mean(sco_train$score2[j])
	#k <- mean(sco_train$score1[j]/sco_train$score2[j])
	sco_train$score1[j] <- (sco_train$score1[j]-mean1)/sd1
	sco_train$score2[j] <- (sco_train$score2[j]-mean2)/sd2
	j <- which(dat$V1==i)
	dat$V4[j] <- as.numeric((dat$V2[j]-mean1)/sd1+(dat$V3[j]-mean2)/sd2)
	dat$V2[j] <- as.numeric(scale(dat$V2[j]))
	dat$V3[j] <- as.numeric(scale(dat$V3[j]))
	#dat$V4[j] <- dat$V2[j]+dat$V3[j]
}

library(pROC)

pdf(paste('auc.1.train.test.pdf',sep="."),width=4,height=4)
h <- 1
C <- array()
for(i in unique(dat$V1)){
	j <- which(sco_train$round==i)
	val <- sco_train$score1[j];
	typ <- rep("Malignant",length(val))
	typ[sco_train$type[j]=="B"] <- "Benign"
	A <- roc(typ,val)
	C <- c(C,A$auc)
	if(h>0){
		h <- 0
		plot(A ,grid=c(0.1, 0.2),col="grey")
	}else{
		lines(A$specificities,A$sensitivities,lwd=2,col="grey")
	}
}
C <- C[-1]
print(t.test(C))
A <- roc(dat[,9],dat[,2])
print(A)
lines(A$specificities,A$sensitivities,lwd=2,col="red")
dev.off()

pdf(paste('auc.-1.train.test.pdf',sep="."),width=4,height=4)
h <- 1
C <- array()
for(i in unique(dat$V1)){
	j <- which(sco_train$round==i)
	val <- sco_train$score2[j];
	typ <- rep("Malignant",length(val))
	typ[sco_train$type[j]=="B"] <- "Benign"
	A <- roc(typ,val)
	C <- c(C,A$auc)
	if(h>0){
		h <- 0
		plot(A ,grid=c(0.1, 0.2),col="grey")
	}else{
		lines(A$specificities,A$sensitivities,lwd=2,col="grey")
	}
}
C <- C[-1]
print(t.test(C))
A <- roc(dat[,9],dat[,3])
print(A)
lines(A$specificities,A$sensitivities,lwd=2,col="red")
dev.off()

pdf(paste('auc.0.train.test.pdf',sep="."),width=4,height=4)
h <- 1
C <- array()
for(i in unique(dat$V1)){
	j <- which(sco_train$round==i)
	val <- sco_train$score1[j]+sco_train$score2[j]
	typ <- rep("Malignant",length(val))
	typ[sco_train$type[j]=="B"] <- "Benign"
	A <- roc(typ,val)
	C <- c(C,A$auc)
	if(h>0){
		h <- 0
		plot(A ,grid=c(0.1, 0.2),col="grey")
	}else{
		lines(A$specificities,A$sensitivities,lwd=2,col="grey")
	}
}
C <- C[-1]
print(t.test(C))
A <- roc(dat[,9],dat[,4])
print(A)
lines(A$specificities,A$sensitivities,lwd=2,col="red")
dev.off()

showROC <-function(typ,val,nme){
	A <- roc(typ,val,ci=T)
	write(paste(nme,A$auc,A$sensitivities[which.min(abs(A$specificities-0.95))],A$sensitivities[which.min(abs(A$specificities-0.9))],sep="\t"),file=stdout())
	pdf(paste('auc',nme,'cor.pdf',sep="."),width=4,height=4)
	plot(A ,print.auc=TRUE, auc.polygon=TRUE,grid=c(0.1, 0.2),max.auc.polygon=TRUE,print.thres=TRUE)
	dev.off()
}

typ <- dat[,9]
val <- as.numeric(dat[,2])
showROC(typ,val,"1")


val <- as.numeric(scale(dat[,3]))
showROC(typ,val,"-1")

val <- as.numeric((dat[,4]))
showROC(typ,val,"0")

typ <- as.character(dat[,10]) ; typ[typ=='IA' | typ=='IB'] <- "I" ; typ[typ=='IIA' | typ=='IIB'] <- "II" ; typ[typ=='IIIA' | typ=='IIIB' | typ=='IIIC'] <- "III" ; typ[typ=='IVA' | typ=='IVB'] <- "IV"
ind <- which(typ=="I"|typ=="None")
ty <- dat[ind,9]
va <- val[ind]
showROC(ty,va,"combined.I")

ind <- which(typ=="II"|typ=="None")
ty <- dat[ind,9]
va <- val[ind]
showROC(ty,va,"combined.II")

ind <- which(typ=="III"|typ=="IV"|typ=="None")
ty <- dat[ind,9]
va <- val[ind]
showROC(ty,va,"combined.III-IV")

ind <- which(dat[,8]!="SC")
ty <- dat[ind,9]
va <- val[ind]
showROC(ty,va,"combined.AD")

ind <- which(dat[,8]!="AD")
ty <- dat[ind,9]
va <- val[ind]
showROC(ty,va,"combined.SC")


val <- dat[,4]
ind <- which(!is.na(dat[,18]) & as.numeric(dat[,21])<=10)
ty <- dat[ind,9]
va <- val[ind]
showROC(ty,va,"less.than.1")

ind <- which(!is.na(dat[,18]) & as.numeric(dat[,21])<=20 & as.numeric(dat[,21])>10)
ty <- dat[ind,9]
va <- val[ind]
showROC(ty,va,"less.than.2")


ind <- which(!is.na(dat[,18]) & as.numeric(dat[,21])>20)
ty <- dat[ind,9]
va <- val[ind]
showROC(ty,va,"more.than.2")
