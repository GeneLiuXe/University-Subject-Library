%--------------------------- header --------------------------
% RoboticsLaTeX v1.0
% By:
% Barbara Bruno <barbara.bruno@unige.it>
% Fulvio Mastrogiovanni <fulvio.mastrogiovanni@unige.it>
% Version 1.0 released on 27/10/2014
%
% based on:
% CUEDthesis v1.0
% By Harish Bhanderi <harish.bhanderi@cantab.net>
%
%-------------------------- identification ---------------------
\ProvidesClass{Classes/ThesisLaTeX}[2014/10/27 LaTeX theses class]

%-------------------------- initial code -----------------------
%-----------------------------report --------------------------
% If you want to write a REPORT uncomment the following lines and comment the "book" lines:
%\DeclareOption*{\PassOptionsToClass{\CurrentOption}{report}}
%\ProcessOptions\relax
%%\LoadClass[a4paper]{report}
%\ifx\pdfoutput\undefined
%   \LoadClass[dvips, a4paper]{report}
%\else
%   \LoadClass[pdftex, a4paper]{report}
%\fi
%%Bibliography
%\renewcommand{\refname}{References}
%------------------------------book ---------------------------
% If you want to write a BOOK uncomment the following lines and comment the "report" lines:
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{book}}
\ProcessOptions\relax
\ifx\pdfoutput\undefined
   \LoadClass[dvips, a4paper]{book}
\else
   \LoadClass[pdftex, a4paper]{book}
\fi
%Useful packages
\usepackage{StyleFiles/watermark}
\usepackage{amssymb}
\usepackage{graphics}
\usepackage{StyleFiles/fancyhdr}
\usepackage{eucal}
\usepackage[style = chem-acs, backend=biber, urldate=long,
            dateabbrev=false]{biblatex}
\addbibresource{References/references.bib}
\usepackage[english]{babel}
\usepackage[usenames, dvipsnames]{color}
\usepackage[perpage]{footmisc}
\usepackage{ifthen}
\usepackage{ifpdf}
%Bibliography
\renewcommand{\bibname}{References}
% NOTE: this command may be overridden by later defined bibliography style 

% Nomenclature
\usepackage{nomencl}
\makeglossary
\renewcommand\nomgroup[1]{%
  \ifthenelse{\equal{#1}{A}}{%
   \item[\textbf{Roman Symbols}] }{%             A - Roman
    \ifthenelse{\equal{#1}{G}}{%
     \item[\textbf{Greek Symbols}]}{%             G - Greek
      \ifthenelse{\equal{#1}{R}}{%
        \item[\textbf{Superscripts}]}{%              R - Superscripts
          \ifthenelse{\equal{#1}{S}}{%
           \item[\textbf{Subscripts}]}{{%             S - Subscripts
	    \ifthenelse{\equal{#1}{X}}{%
	     \item[\textbf{Other Symbols}]}{{%    X - Other Symbols
	    \ifthenelse{\equal{#1}{Z}}{%
	     \item[\textbf{Acronyms}]}%              Z - Acronyms
              			{{}}}}}}}}}}
\ifpdf

% Figure options
    \usepackage[ pdftex, plainpages = false, pdfpagelabels, 
                 pdfpagelayout = useoutlines,
                 bookmarks,
                 bookmarksopen = true,
                 bookmarksnumbered = true,
                 breaklinks = true,
                 linktocpage,
                 colorlinks = true,
                 linkcolor = black,
                 urlcolor  = black,
                 citecolor = black,
                 anchorcolor = black,
                 hyperindex = false,
                 hyperfigures
                 ]{hyperref} 

    \DeclareGraphicsExtensions{.png, .jpg, .pdf}
    \usepackage[pdftex]{graphicx}
    \pdfcompresslevel=9
    \graphicspath{{Figures/PNG/}{Figures/PDF/}{Figures/System/}{Figures/}}
\else
    \usepackage[ dvips, 
                 bookmarks,
                 bookmarksopen = true,
                 bookmarksnumbered = true,
                 breaklinks = true,
                 linktocpage,
                 pagebackref,
                 colorlinks = true,
                 linkcolor = black,
                 urlcolor  = black,
                 citecolor = black,
                 anchorcolor = black,
                 hyperindex = false,
                 hyperfigures
                 ]{hyperref}

    \DeclareGraphicsExtensions{.eps, .ps}
    \usepackage{epsfig}
    \usepackage{graphicx}
    \graphicspath{{Figures/EPS/}{Figures/System/}{Figures/}}
\fi

% Page settings (a4 paper)
\ifpdf
   \pdfpageheight=297mm
   \pdfpagewidth=210mm
\else
   \setlength{\paperheight}{297mm}
   \setlength{\paperwidth}{210mm}
\fi
\setlength{\hoffset}{0.00cm}
\setlength{\voffset}{0.00cm}
\setlength{\evensidemargin}{1.96cm}
\setlength{\topmargin}{1mm}
\setlength{\headheight}{1.36cm}
\setlength{\headsep}{1.00cm}
\setlength{\textheight}{20.84cm}
\setlength{\textwidth}{14.5cm}
\setlength{\marginparsep}{1mm}
\setlength{\marginparwidth}{3cm}
\setlength{\footskip}{2.36cm}

\pagestyle{fancy}
\renewcommand{\chaptermark}[1]{\markboth{\MakeUppercase{\thechapter. #1 }}{}}
\renewcommand{\sectionmark}[1]{\markright{\thesection\ #1}}
\fancyhf{}
\fancyhead[RO]{\bfseries\rightmark}
\fancyhead[LE]{\bfseries\leftmark}
\fancyfoot[C]{\thepage}
\renewcommand{\headrulewidth}{0.5pt}
\renewcommand{\footrulewidth}{0pt}
\addtolength{\headheight}{0.5pt}
\fancypagestyle{plain}{
  \fancyhead{}
  \renewcommand{\headrulewidth}{0pt}
}

%-------------------------- commands -----------------------
\newcommand{\submittedtext}{{}}
\newcommand{\submittedtextt}{{\textit{Supervisor}}}
\newcommand{\supervisor}[1]{\def\@supervisor{#1}}

% Declarations
% The year and month the degree will be officially conferred
\def\degreedate#1{\gdef\@degreedate{#1}}
% The full (unabbreviated) name of the degree
\def\degree#1{\gdef\@degree{#1}}
% The name of the department (eg. DIBRIS)
\def\collegeordept#1{\gdef\@collegeordept{#1}}
% The name of the university  (eg. University of Genova)
\def\university#1{\gdef\@university{#1}}
% University crest
\def\crest#1{\gdef\@crest{#1}}

%\def\supervisor#1{\gdef\@supervisor{#1}}

% Special pages (single column always)
\newenvironment{alwayssingle}{%
       \@restonecolfalse\if@twocolumn\@restonecoltrue\onecolumn
       \else\newpage\fi}
       {\if@restonecol\twocolumn\else\newpage\fi}

% TITLE PAGE
\renewcommand{\maketitle}{%
\begin{alwayssingle}
    \renewcommand{\footnotesize}{\small}
    \renewcommand{\footnoterule}{\relax}
    \thispagestyle{empty}
  \begin{center}
    { \Huge {\bfseries {\@title}} \par}
{\large \vspace*{25mm} {{\@crest} \par} \vspace*{25mm}}
    {{\Large \@author} \par}
{\large \vspace*{1ex}
    {{\@collegeordept} \par}
\vspace*{1ex}
    {{\@university} \par}
\vspace*{5mm}
    { {\submittedtextt} \par}
\vspace*{2mm}
    { {\@supervisor} \par}    
\vspace*{10mm}
    {{\submittedtext} \par}
\vspace*{1ex}
    {\it {\@degree} \par}
\vspace*{2ex}
    {\@degreedate}}
  \end{center}
  \null\vfill
\end{alwayssingle}}

% DEDICATION
% Dedication is on a separate page and set out in verse format.
\newenvironment{dedication}
{\begin{alwayssingle}
  \pagestyle{empty}
  \begin{center}
  \vspace*{1.5cm}
  {\LARGE }
  \end{center}
  \vspace{0.5cm}
  \begin{quote} \begin{center}}
{\end{center} \end{quote} \end{alwayssingle}}

% ACKNOWLEDGEMENTS
%----------- short --------
% Acknowledgements are on a separate page, with content appearing in a quote environment and title "Acknowledgements" in large, bold, centered style.
\newenvironment{acknowledgements}
{\pagestyle{empty}
\begin{alwayssingle}
\begin{center}
\vspace*{1.5cm}
{\Large \bfseries Acknowledgements}
\end{center}
\vspace{0.5cm}
\begin{quote}}
{\end{quote}\end{alwayssingle}}
%----------- long ---------
% Acknowledgements are on a separate page, with free content and title "Acknowledgements" in large, bold, centered style.
\newenvironment{acknowledgementslong}
{\pagestyle{empty}
\begin{alwayssingle}
\begin{center}
\vspace*{1.5cm}
{\Large \bfseries Acknowledgements}
\end{center}
\vspace{0.5cm}\begin{quote}}
{\end{quote}\end{alwayssingle}}

% ABSTRACT
%--- short ---
% Abstract is on a separate page, with content appearing in a quote environment and title "Abstract" in large, bold, centered style.
\newenvironment{abstracts} {\begin{alwayssingle} \pagestyle{empty}
  \begin{center}
  \vspace*{1.5cm}
  {\Large \bfseries  Abstract}
  \end{center}
  \vspace{0.5cm}
   \begin{quote}}
{\end{quote}\end{alwayssingle}}
%--- long ----
% Abstract is on a separate page, with free content and title "Abstract" in large, bold, centered style.
\newenvironment{abstractslong} {\begin{alwayssingle} \pagestyle{empty}
  \begin{center}
  \vspace*{1.5cm}
  {\Large \bfseries  Abstract}
  \end{center}
  \vspace{0.5cm} \begin{quote}}
{\end{quote}\end{alwayssingle}}
%- separate -
% Abstract is on a separate page, with running title and author.
\newenvironment{abstractseparate} {\begin{alwayssingle} \pagestyle{empty}
  \vspace*{-1in}
 \begin{center}
    { \Large {\bfseries {\@title}} \par}
    {{\large \vspace*{1ex} \@author} \par}
{\large \vspace*{1ex}
    {{\@collegeordept} \par}
    {{\@university} \par}
\vspace*{1ex}
    {{\it \submittedtext} \par}
    {\it {\@degree} \par}
\vspace*{2ex}
    {\@degreedate}}
  \end{center}}
{\end{alwayssingle}}

% ROMANPAGES
% Page numbering with lowercase roman numbers for the contents and figure lists.
% Page numbering with reset (starting from 1) arabic page numbers for the remainder of the document.
\newenvironment{romanpages}
{\setcounter{page}{1}
  \renewcommand{\thepage}{\roman{page}}}
{\newpage\renewcommand{\thepage}{\arabic{page}}\setcounter{page}{1}}

