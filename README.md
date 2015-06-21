Master's thesis
===============

### Name

###### master-thesis

### Description

My computer science master's thesis in network and distributed systems. 

### Thesis title
###### Graphs as a primitive on the control plane to analyse and manage software defined networks

### Thesis abstract

> Software Defined Networking ([SDN](https://www.opennetworking.org/sdn-resources/sdn-definition)) is an emergent architecture
> that is dynamic, flexible, manageable and with low cost,
> consistent with the dynamics of the modern applications.
> This architecture decouples the control plane from the data plane.
> Tipically, networks are represented as [graphs](https://en.wikipedia.org/wiki/Graph_theory).
> Based on it, this paper presents a network
> representation model using a graph through the control plane
> of a [SDN controller](https://www.sdxcentral.com/resources/sdn/sdn-controllers/).
> Our SDN solution adopts OpenFlow protocol.
> This project is based on [POX](https://github.com/noxrepo/pox), a SDN controller compatible
> with [OpenFlow](https://www.opennetworking.org/sdn-resources/openflow) devices.
> The graph aproach provides a global view of the network in real
> time and consistency.
> The experiments show that the graph is a reliable representation of
> the real network and its events ocurred along time inside the network,
> thus easing the management in Software Defined Networking.

### Keywords

Distributed Systems, Software Defined Networking, Control Plane, Software Design

### Project structure

This project is written in [LaTex](http://www.latex-project.org/).
There is two folders in this project as follows. 
'thesis' has the master's thesis document using the [PPGCC](https://www.dcc.ufmg.br/pos/alunos/modelodisstese.php) template.
'presentation' has the slides using the [TUDelft Beamer](http://www.ewi.tudelft.nl/en/the-faculty/departments/applied-mathematics/latex/beamer/) template.
Inside each directory there is a [Makefile](https://www.gnu.org/prep/standards/html_node/Makefile-Conventions.html) that implement rules 
to build, run and clean projects.
The results of projects build are PDF files: thesis.pdf and presentation.pdf.

### Makefile rules

- Build and run project
```bash
$> make latex
```

- Build project
```bash
$> make
```

- Run (read) project
```bash
$> make read
```

- Clean project
```bash
$> make clean
```

### Author

> ###### Written by Gustavo Pantuza

### Reporting Bugs

> ###### Report Panfiles bugs to gustavopantuza@gmail.com
