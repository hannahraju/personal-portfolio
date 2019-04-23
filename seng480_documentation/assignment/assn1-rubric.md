# Marking Scheme - Assignment 1

## Total [21/25]

## Overall marks [5/5]
- technical writing: organization, sentence structure, clarity
- evidence backing up your conclusions
- introduction that explains the project and scenarios

## Scenarios [16/20]
- (2) [template version](https://github.com/SENG480-18/course/blob/master/lectures/4-req.md#scenario-generation) of each scenario, outlining stimulus, response measure, business goals (as with M2).
	- Clearly explain whether this is a use case or growth scenario.
	- Include some evidence supporting your choice of response measure.
- (4) Outline of investigation approach (lab notebook style, explaining the steps you took)
- (8) 2 Diagrams <s>view</s> + Keys, explaining the project-specific use case, and growth, scenario (e.g. 'Uses', 'Deployment', etc.).
- (6) 2 Text descriptions answering question: <font color="red">does the current design meet the response measure (for use case|growth scenarios). </font>
	- Explain with reference to the diagram.
	- Include evidence supporting your conclusions: either on the diagram (e.g. annotate a link with properties such as 'processing time'). Back up the evidence with reference to specific artifacts from the project: code, deployment docs, Jenkins servers, etc.

## Notes
(Explaining why marks were deducted)

- Using time as a response measure does not reflect architectural changes, which is what modifiability focuses on. A more convenient measure would be how many other changes would you need to make to the system as a result of your initial change. (-1)
- So, figure 1 is a generalization/specialization diagram? Keep class names consistent so readers can map them to the source code.
- Your description of the QAS is fairly high-level (-1)
- Your growth scenario suffers form the same response measure issue. (-1)
- How realistic is it that this many expansion packs come out at the same time?
- Your description of the growth scenario is also high-level (-1)
