arab_bam_30contigs, this folder contains the following:

Reads
arabidopsis30Contigs.fa
bam_files
compacta_files
subset.txt

./Reads:
sample_01_1.fasta
sample_01_2.fasta
sample_02_1.fasta
sample_02_2.fasta
sample_03_1.fasta
sample_03_2.fasta

./bam_files:
arsample1.bam
arsample2.bam
arsample3.bam

./compacta_files:
arabSample.comed
arabSample.comtr


Reads. 3 samples of Paired-end reads simulated from 30 randomly selected genes of Arabidopsis.

arabidopsis30Contigs.fa. Fasta file with the nucleotide sequence of 30 randomly selected genes of Arabidopsis,this file is used as reference.

bam_files. Three bam files result of mapping the reads (from the Reads folder) to the reference (arabidopsis30Contigs.fa) using bowtie2.

compacta_files. This set of comed and comtr files are the output of using the -p true option in Compacta, they store the preprocessed data (from the bam files), in an easy and quick to load format.

We have covered most of Compacta's input options, depending on what you want to do, you could either go from mapping the read to the reference to produce bam files or use the included bam files or preprocessed files, note that Compacta's output may vary depending on alignment algorithm and parameters, we recommend using a strict policy (like '--very-sensitive --score-min L,-0.1,-0.1’ on bowtie2), and reporting multi-mapping reads as they are used by Compacta to cluster contigs together.

If you want to try compacta using the provided bam files, then you should do it like this:
<path to compacta>/Compacta -n 3 -s sample_1,sample_2,sample_3 -b arsample1.bam,arsample2.bam,arsample3.bam -o arabidopsis_30Contigs

If you want to try using Compacta's preprocessed files then you can do it like this:

<path to compacta>/Compacta -n 3 -s sample_1,sample_2,sample_3 -c arabSample  -o arabidopsis_30Contigs

Both options should produce three files with suffix _clusters.txt,_counts.txt and _precluster.txt.
_clusters.txt contains the contig-cluster mapping; a 1 if the contig is representative of its cluster or a 0 otherwise; a description of the status of each contig: ZE (Zero evidence) when the contig has no reads mapping to it, SC (Singleton contig) when the contig doesnt share reads with another contig; SL (Singleton by Low Evidence) contigs with few reads that were disconnected from others because of its reduced evidence; NC Contigs clustered with at least another contig; the length of the contig and the number of reads mapped to the contig.
_counts.txt contains the raw counts for each cluster and each sample.
_preclusters.txt contains the definition of the preclusters calculated by Compacta before running the clustering algorithm.


head arabidopsis_30Contigs_*
==> arabidopsis_30Contigs_clusters.txt <==
AT1G32415.1	Cluster.0	1	SC	2286	83
AT1G33030.1	Cluster.1	1	SC	1232	52
AT1G33055.1	Cluster.2	1	SC	1271	80
AT1G33835.1	Cluster.3	1	SC	451	69
AT1G34245.1	Cluster.4	1	SC	608	74
AT1G34290.1	Cluster.5	1	SC	801	59
AT1G34044.1	Cluster.6	1	SC	597	71
AT1G35340.4	Cluster.7.0	0	NC	1184	198
AT1G35340.1	Cluster.7.0	1	NC	1115	230
AT1G35340.2	Cluster.7.0	0	NC	1138	230

==> arabidopsis_30Contigs_counts.txt <==
Cluster_ID	sample_1	sample_2	sample_3
Cluster.0	20.000000	39.000000	24.000000
Cluster.1	19.000000	17.000000	16.000000
Cluster.2	26.000000	26.000000	28.000000
Cluster.3	21.000000	31.000000	17.000000
Cluster.4	21.000000	23.000000	30.000000
Cluster.5	20.000000	16.000000	23.000000
Cluster.6	31.000000	23.000000	17.000000
Cluster.7.0	94.000000	96.000015	90.000008
Cluster.8.0	51.000000	62.000000	60.000000

==> arabidopsis_30Contigs_preclusters.txt <==
 Precluster_ID	nContigs	contigIDs
7	4	AT1G35340.4,AT1G35340.1,AT1G35340.2,AT1G35340.3
8	2	AT1G35420.1,AT1G35420.2
9	3	AT1G34120.1,AT1G34120.2,AT1G34120.3
10	5	AT1G32410.1,AT1G32410.2,AT1G32410.3,AT1G32410.4,AT1G32410.5
11	5	AT1G33970.1,AT1G33970.2,AT1G33970.3,AT1G33970.4,AT1G33970.5
12	4	AT1G33050.1,AT1G33050.2,AT1G33050.3,AT1G33050.4


A subset file can be used for testing and to fuse transcriptomes, in this example we are using the subset.txt file which contains only 10 of the 30 contigs in the experiment.

cat ../subset.txt
AT1G34290.1
AT1G34044.1
AT1G35340.4
AT1G35340.1
AT1G35340.2
AT1G33970.5
AT1G33050.1
AT1G33050.2
AT1G33050.3
AT1G33050.4

