
#!/usr/bin/env Rscript

library(dplyr, warn.conflicts = FALSE)
library(tidyr, warn.conflicts = FALSE)
library(ggplot2)
library(caret, warn.conflicts = FALSE)
library(e1071)
library(nnet)


args = commandArgs(trailingOnly=TRUE)

if (length(args)==0) {
  stop("At least one argument must be supplied (input file)", call.=FALSE)
} else if (length(args)==1) {

}


seismic <- read.csv(args[1])

#Subsetting and Clean-Up
seismic <- seismic %>%
    dplyr::select(-magType, -nst, -gap, -dmin, -net, -updated) %>%
        mutate(type = factor(type)) %>%
            filter(latitude < as.double(args[2]),
                   latitude > as.double(args[3]),
                   longitude < as.double(args[4]),
                   longitude > as.double(args[5])) %>%
    dplyr::select(type, mag, depth, rms) %>%
    na.omit()

#filter landslide
seismic <- seismic %>% filter(type != "landslide") %>%
    mutate(type = factor(type))

seismic$type <- factor(ifelse(seismic$type == "anthropogenic event" |
                              seismic$type == "mining explosion",
                              "explosion", as.character(seismic$type)))

seismic[, -1] <- scale(seismic[, -1])
set.seed(100)
sampl <-  c(
    (data.frame(which(seismic$type == "earthquake")) %>%
        sample_frac(0.8))[,1],
    (data.frame(which(seismic$type == "explosion")) %>%
        sample_frac(0.8))[,1],
    (data.frame(which(seismic$type == "quarry blast")) %>%
        sample_frac(0.8))[,1])
seismic.train <- seismic[sampl,]
seismic.test <- seismic[-sampl,]
x.train <- as.matrix(seismic.train[,-1])
y.train <- seismic.train[,1]
x.test <- as.matrix(seismic.test[,-1])
y.test <- seismic.test[,1]

#modeling
(seismic.multinom <- multinom(type ~ depth + mag + rms, seismic.train))

#predic
multinom.pred <- predict(seismic.multinom, newdata = seismic.test, type = "class")
err <- confusionMatrix(multinom.pred, y.test)
err$table

