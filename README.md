# ground_floor

Groud Floor is used to record skills and job acheivements. 
Using a template will print out the wanted material into a tex document.
Use LaTeX to generate a pdf off of the tex doc type.

Current working example of a TeX template. 
```
\documentclass{article}
\begin{document}

\section*{<<NAME>>}

<<BIO>>

\section*{Skills}
\begin{itemize}
%BEGIN_SKILL%
\item <<SKILL>>
%END_SKILL%
\end{itemize}

\section*{Experience}
%BEGIN_JOB%
\textbf{<<COMPANY>>} \\
<<POSITION>> \\
<<STARTDATE>> -- <<ENDDATE>> \\[6pt]
%END_JOB%

\end{document}

```
