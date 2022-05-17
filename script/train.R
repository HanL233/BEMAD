args <- commandArgs();
if(length(args) != 7 || !file.exists(args[6]) || !file.exists(args[7])){
        write(paste("Rscript",unlist(strsplit(args[4],"="))[2],"<FI:data.txt>","<FI:data.txt>",sep=" "),stderr());
        q();
}

file1 <- args[6]
file2 <- args[7]

dat1 <- readLines(file1)
dat2 <- readLines(file2)

typ1 <- unlist(lapply(dat1,function(x){(unlist(strsplit(x,"\t"))[1])}))
typ2 <- unlist(lapply(dat2,function(x){(unlist(strsplit(x,"\t"))[1])}))

ty <- unique(typ1)

for(i in ty){
	da1 <- array()
	ind1 <- which(typ1==i)
	for(x in ind1){
		da1 <- rbind(da1,as.numeric((unlist(strsplit(dat1[x],"\t")))[-1]))
	}
	da1 <- t(da1[-1,])

	da2 <- array()
	ind2 <- which(typ2==i)
	for(x in ind2){
		da2 <- rbind(da2,as.numeric((unlist(strsplit(dat2[x],"\t")))[-1]))
	}
	da2 <- t(da2[-1,])
	
	dat <- cbind(da1,da2)
	lyr <- as.numeric(scale(as.numeric(apply(dat,1,mean))))
	wgt <- as.numeric(apply(dat,1,var))
	A <- abs(as.numeric(apply(da1,2,function(x){sum(((scale(x)-lyr)*wgt*wgt))})))
	B <- abs(as.numeric(apply(da2,2,function(x){sum(((scale(x)-lyr)*wgt*wgt))})))
	write(paste(i,t.test(A,B)$p.value,mean(A),mean(B),sep="\t"),file=stderr())
	write(paste(i,paste(c(lyr,wgt),collapse=","),sep=","),file=stdout())
}
