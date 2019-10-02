arab_fasta_histones, this folder contains the following files and folders:

Compacta_Files
Reads
arabidopsisHistones.fa
bam_Files

./Compacta_Files:
arabHistones.comed
arabHistones.comtr

./Reads:
sample_01_1.fasta
sample_01_2.fasta
sample_02_1.fasta
sample_02_2.fasta
sample_03_1.fasta
sample_03_2.fasta

./bam_Files:
arab_histone_sample1.bam
arab_histone_sample2.bam
arab_histone_sample3.bam


Reads. 3 samples of Paired-end reads simulated from selected histone genes of Arabidopsis.

arabidopsisHistones.fa. Fasta file with the nucleotide sequence of 41 histone genes of Arabidopsis,this file is used as reference.

bam_files. Three bam files result of mapping the reads (from the Reads folder) to the reference (arabidopsisHistones.fa) using bowtie2.

compacta_files. This set of comed and comtr files are the output of using the -p true option in Compacta, they store the preprocessed data (from the bam files), in an easy and quick to load format.

We have covered most of Compacta's input options, depending on what you want to do you could either go from mapping the read to the reference to produce bam files or use the included bam files or preprocessed files, note that Compacta's output may vary depending on alignment algorithm and parameters, we recommend using a strict policy (like '--very-sensitive --score-min L,-0.1,-0.1’ on bowtie2), and reporting multi-mapping reads as they are used by Compacta to cluster contigs together.

If you want to try compacta using the provided bam files, then you should do it like this:
<path to compacta>/Compacta -n 3 -s sample_1,sample_2,sample_3 -b arab_histone_sample1.bam,arab_histone_sample2.bam,arab_histone_sample3.bam -o arab_histone_test

If you want to try using Compacta's preprocessed files then you can do it like this:

<path to compacta>/Compacta -n 3 -s sample_1,sample_2,sample_3 -c arabHistones -o arab_histone_test

Both options should produce three files with suffix _clusters.txt,_counts.txt and _status.txt.
_clusters.txt contains the contig-cluster mapping; a 1 if the contig is representative of its cluster or a 0 otherwise; a description of the status of each contig: ZE (Zero evidence) when the contig has no reads mapping to it, SC (Singleton contig) when the contig doesnt share reads with another contig; SL (Singleton by Low Evidence) contigs with few reads that were disconnected from others because of its reduced evidence; NC Contigs clustered with at least another contig; the length of the contig and the number of reads mapped to the contig.
_counts.txt contains the raw counts for each cluster and each sample.
_preclusters.txt contains the definition of the preclusters calculated by Compacta before running the clustering algorithm.

head arab_histone_test_*
==> arab_histone_test_clusters.txt <==
AT1G09200.1	Cluster.0	1	SC	607	91
AT1G08880.1	Cluster.1	1	SC	748	87
AT1G08170.1	Cluster.2	1	SC	732	86
AT1G75610.1	Cluster.3	1	SC	584	92
AT1G75600.1	Cluster.4	1	SC	531	78
AT1G13370.1	Cluster.5	1	SC	648	48
AT1G07790.1	Cluster.6	1	SC	729	78
AT1G21970.1	Cluster.7	1	SC	936	82
AT2G37470.1	Cluster.8	1	SC	605	57
AT2G28720.1	Cluster.9	1	SC	701	65

==> arab_histone_test_counts.txt <==
Cluster_ID	sample_1	sample_2	sample_3
Cluster.0	25.000000	35.000000	31.000000
Cluster.1	26.000000	31.000000	30.000000
Cluster.2	22.000000	37.000000	27.000000
Cluster.3	32.000000	28.000000	32.000000
Cluster.4	26.000000	22.000000	30.000000
Cluster.5	15.000000	18.000000	15.000000
Cluster.6	32.000000	23.000000	23.000000
Cluster.7	36.000000	30.000000	16.000000
Cluster.8	13.000000	23.000000	21.000000

==> arab_histone_test_preclusters.txt <==
 Precluster_ID	nContigs	contigIDs
28	4	AT1G07660.1,AT1G07660.2,AT1G07820.1,AT1G07820.2
29	2	AT1G01370.1,AT1G01370.2
30	2	AT3G45930.1,AT3G46320.1
31	3	AT4G40030.1,AT4G40030.2,AT4G40030.3
32	2	AT4G40040.2,AT4G40040.1