\ProvidesClass{template}
\LoadClass{article}

\usepackage{mathtools}
\usepackage[margin=1in]{geometry}
\usepackage{listings}
\usepackage{quoting}
\usepackage{mdframed}
\usepackage{graphicx}
\usepackage{xcolor}
\usepackage{hyperref}
\usepackage{hypcap}
\usepackage{seqsplit}
\usepackage{tabularx}

\hypersetup{
  pdftex,
  linktoc=all
}

\lstset{language=C++,
  basicstyle=\small,
  keywordstyle=\color{blue}\small,
  stringstyle=\color{red}\small,
  commentstyle=\color{gray}\small,
  frame=single,
  showstringspaces=false,
  numbers=left,
  breaklines=true,
  showspaces=false,
  showstringspaces=false,
  showtabs=false,
  tabsize=4
}

\newenvironment{blockquote}{
  \begin{mdframed}[linewidth=3pt,topline=false,rightline=false,bottomline=false]
    \begin{quoting}[indentfirst=false]
      \itshape
    }{ \end{quoting} \end{mdframed} }

\newcommand{\hwproblem}[2]{
  \subsubsection{Problem #1}
  \begin{blockquote}
    #2
  \end{blockquote}
}

\newcommand{\srccode}[1]{
  \lstinputlisting[caption=#1]{#1}
}

\newcommand{\srcfile}[1]{
  \lstinputlisting[caption=#1,language=bash]{#1}
}

\newcommand{\info}[3]{
  \begin{tabularx}{\textwidth}{lX}
  Name: & Nicolas Nytko \hfill Course: CSC216\\\\
  Activity: & #1\\
  Level: & #2\\
  Description: & #3
\end{tabularx}
}