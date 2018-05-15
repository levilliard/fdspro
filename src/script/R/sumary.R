#!/usr/bin/env Rscript

args = commandArgs(trailingOnly=TRUE)

if (length(args)==0) {
  stop("At least one argument must be supplied (input file)", call.=FALSE)
} else if (length(args)==1) {

}

# all parametters
	# 1 seismic file name
	# lat max
	# lat min
	# lon max
	# lon min

library(dplyr, warn.conflicts = FALSE)
library(tidyr, warn.conflicts = FALSE)
library(ggplot2)
options(digits=5)

seismic <- read.csv(args[1])
seismic <- seismic %>%
    dplyr::select(-magType, -nst, -gap, -dmin, -net, -updated) %>%
        mutate(type = factor(type)) %>%
            filter(latitude < as.double(args[2]),
                   latitude > as.double(args[3]),
                   longitude < as.double(args[4]),
                   longitude > as.double(args[5])) %>%
    dplyr::select(type, mag, depth, rms) %>%
    na.omit()
summary(seismic)

p<-ggplot(seismic, aes(x = type, fill = type)) +geom_bar()+coord_flip()
plot(p)
ggsave(args[6])


