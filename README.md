# HY-540: Advanced Topics In Programming Languages Development - Project

## Information

This repository is for the project of the course HY-540: Advanced Topics In Programming Languages Development. It will be used as following:

* We will all have one **branch** named after our credentials for developing. These branches will be only for their "owner" and we are allowed to make any mistakes there.
* The master branch will contain the latest stable version of our project. All of us are allowed to update the master branch with changes from other branches but only when we are **certain** that everything works as expected.
* The master branch will only contain, at any time, the **final** version of the project. Versions that arise during development will stay in their branches until they are finished. Therefore, the code that we will submit will be the code in the master branch.
* We will use **tags** to indicate the process of development.

## Prerequisites

Required software:

* g++ v7.4.0

Required hardware:

* N/A

Required Files:

* *See Project Stucture*.

## Installation

To download project:
```bash
git clone https://gitlab.com/myronfirst/hy540_project.git
```

To build the interpreter:
```bash
cd hy540_project/
make
```

## Project Structure
```
.
├── docs/
├── src/
├── unused/
├── tests/
├── .gitignore
├── Makefile
└── README.md
```

* docs: Project documentation.
* src: Source code.
* tests: A collection of tests files to be used as input.
* unused: Snippets of code that is no longer used.

## Notes

* Please note that the makefile is custom-made so it might fail under conditions that have not been tested. In such cases please build it by hand.

* Please run "$ **make clear**" before committing anything.

## Lines of Code

* 22 files
* 881 blank
* 117 comment
* 3382 code

## Roadmap
See the open issues for a list of proposed features (and known issues).

## Contributors / Authors

* Agapakis Manolis
* Katsarakis Alexandros
* Papadogiannakis Manos
* Tsatsarakis Myron

*In alphabetic order*

## Support

Please contact one of the project's contributors.

If you need an expert:
Anthony Savvidis (as@ics.forth.gr)

## License

Closed source. **Proprietary and confidential**.
Unauthorized copying of this project, via any medium is strictly prohibited.