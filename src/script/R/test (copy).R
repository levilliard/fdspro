#!/usr/bin/env Rscript

args = commandArgs(trailingOnly=TRUE)

if (length(args)==0) {
  stop("At least one argument must be supplied (input file)", call.=FALSE)
} else if (length(args)==1) {
  args[2] = "out.txt"
}

library(dplyr, warn.conflicts = FALSE)
library(tidyr, warn.conflicts = FALSE)
library(ggplot2)
seismic <- read.csv(args[1])
seismic <- seismic %>%
    dplyr::select(-magType, -nst, -gap, -dmin, -net, -updated) %>%
        mutate(type = factor(type)) %>%
            filter(latitude < 49,
                   latitude > 18,
                   longitude < -63.37,
                   longitude > -124.6) %>%
    dplyr::select(type, mag, depth, rms) %>%
    na.omit()
summary(seismic)

ggplot(seismic, aes(x = type, fill = type)) +geom_bar()+coord_flip()



