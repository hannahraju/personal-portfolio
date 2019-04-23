---
Title: Marking Rubric - Project
Authors: Neil Ernst, Omar Elazhary
---

# Running Total (this will change each milestone): 82.5

NB: for all milestones, clarity, legibility, and other good technical writing characteristics are vital and marks will be deducted if not followed.

# Milestone 1   [8 / 10]

## Marking Guide
### Stakeholders and Goals 4/5
- completeness and relevance of stakeholders
- table is well formatted and easily legible
- properly thought out business goals, referenced from empirical data (videos, blogs, project docs, tech pubs, etc.)
- at least 5 well developed business goals.

Marks deducted:
- lack of evidence for business goals
- lack of evidence of effort in creating stakeholder table.

### Ethics report 4/5
- proper formatting and references supporting conclusions
- evidence of critical assessment of possible ethical dilemmas
- demonstrated understanding of ACM Ethical Code and how it applies to this project.

## Notes
(explaining why marks were deducted)
- Stakeholder Table:
    - For suppliers, if the project is using open source libraries, the providers of these libraries are technically under no obligation to act as suppliers.
- Business Goals:
    - You list general business goals, but you do not elaborate on them. For instance, how will Ray fulfill a responsibility to society?
- Ethics Report:
    - You raise some valid concerns, but how do those map to the ACM code of ethics?

-----

# Milestone 2   [5.5 / 10]

## Marking Guide
- extensive list of ASRs, bullet form fine. Derived from business goals. (3)
- 7 scenarios in short form (3)
- 3 templated scenarios (of the 7), following the template in the notes (nb: not the full template) (2)
- Utility tree with (H,H) format for prioritization (prioritization should be reasonable but not necessarily referenced) (2)

Marks deducted:
- scenarios seem to have little to no connection with the project
- scenarios seem to have little connection to the previous milestone (business goals)
- poor technical writing
- Quality of scenarios (clear analysis of stimulus, response, response measure)


## Notes M2
(explaining why marks were deducted)
- ASRs (1.5/3):
    - While bullet form is fine, you do still need to elaborate a little bit on these requirements and how they stem from business goals. More words, please.
- QSAs (3/5):
    - Number of tasks per second is not an accurate indicator of a system's performance. Time and resources taken up by the system to perform a particular task are the typical measures used for performance.
    - For your modifiability scenario, the response measure is too vague. The whole scenario is too vague. What gets changed? Based on that you'll be able to measure modifiability.
    - The same applies to your interoperability scenario.
- Utility Tree (1/2):
    - You do not include a legend. I can't tell technical from business complexity.
    - The previous critique to your scenarios still applies here.

-----

# Milestone 3   [20 / 20]

## Marking Guide
- all required sections are present and well-organized. Include a table of contents. (2)
- view's primary presentation (diagram) is thorough and understandable. This means key elements are labeled, relations are typed, color is used appropriately. Use of UML or other diagram notations is not mandatory. (5)
- view and element catalog answers key analysis questions implied by QAS (M2). (5)
- behavior diagram is appropriate and clearly answers important questions from the QAS. (5)
- rationale is well-reasoned and any obvious assumptions are documented. (3)

Potential deductions:
- diagram missing key details and hard to understand in isolation
- no connection to project QA and previous milestones
- disconnect between Github code and your understanding.
- poor technical writing

## Notes M3
(explaining why marks were deducted)

- Why do you mention a `layer/module` in your key, if you never use it?
- No interfaces.

-----

# Milestone 4    [18 / 20]

## Marking Guide
- all required sections are present and well-organized. Include a table of contents.
- view’s primary presentation (diagram) is thorough and understandable. This means key elements are labeled, relations are typed, color is used appropriately. Use of UML or other diagram notations is not mandatory.
- view and element catalog answers key analysis questions implied by QAS (M2).
- behavior diagram is appropriate and clearly answers important questions from the QAS.
- rationale is well-reasoned and any obvious assumptions are documented.

Potential deductions:
- diagram missing key details and hard to understand in isolation
- no connection to project QA and previous milestones
- disconnect between Github code and your understanding.
- poor technical writing
- no evidence of reflection and response to comments in M3

## Notes M4
(explaining why marks were deducted)

- It feels like a sequence diagram would have been more representative of function calls in your primary view.
- Your rationale for the second interface (WebUI) describes the user interface. It is supposed to justify why code was built in this particular way, not describe and evaluate the user interface. Also, interface in this context refers to inter-modular communication. (-2)

# Milestone 5  [17 / 15]

## Marking Guide
- Analytics: you ran tools and identified problems or positive parts of the codebase
- Evidence of tool use.
- Report meets tech writing criteria.
- Connection of analysis to other aspects of the project, if any.
- Technical debt is plausible and not merely the simplest rule violation.
- 2% project bonus for using SonarQube and pointing me to a Sonarqube instance

## Notes M5

- When you say "PyLint report", I was not expecting the tool output dump.
- Link to SonarQube provided. (+2)

-----

# Milestone 6 [ 2 / 5]

## Marking Guide
- **5** marks for presentation,  Gitbook formatted PR with final chapter material
- Presentation organization adequate
- Presentation competently run
- Presentation on time.
- Presentation: questions answered, evidence of in depth knowledge
- Presentation: slides attractive and clear
- Presentation: speaking style clear and evidence of rehearsal

- Report compiles to PDF/Gitbook with no bugs
- Report has all necessary sections.
- Report has fixed comments from previous submissions.
- Report meets tech writing criteria.
- Report has introduction and organization is consistent

## Notes M6
- The PR text was not in the proper place (-1)
- Reading off a phone or index card loses you your audience’s engagement. (-1)
- The utility tree is too big to include on the slide. The text is barely legible. Cropping the tree is good, but it doesn’t fix - the text legibility problems. (-1)
- When you start describing a C&C view, a diagram might be more helpful. Especially if you’re describing interactions “between - objects”.
- The font you use in your C&C view is weird. It feels like the whole view was warped in order to fit onto the slide.
- Using Pylint for evaluation implies the project’s devs are compliant/trying to comply with the python coding style guide. Is - that true? (It’s not all Pylint does, but it is a portion.)

-----

# Milestone 7 [12 / 20]

## Marking Guide
- pull request is interesting and demonstrates knowledge of project architecture.
- PR meets project standards; e.g., not a lot of changes needed by maintainer.
- marks off for triviality or simplicity

## Notes M7
- The PR did not discuss any of the detail about what was involved. 
- The PR did not really solve a problem or add anything substantive. 
- Maintainer questions and build processs were not followed

