# Undo System
- Uses a transaction system (takes a snapshot of the object state)
- Uses RTTI/Reflection/Codegen to serialize objects
- Supports customisation of a property
- Requires knowledge that a modification has finished 
- Must be able to support multi-edit
- Support for multiple inheritance?

## Transactions
- No persistence between sessions (not saved to file)
	- Important because offline changes to files will invalidate any previous session transactions
- Supports 3 types of commands Add, Delete, Set Value
- Should be as simple as `TransactionBegin` and `TransactionEnd`
	- Push/Pop the property name/index to a stack
	- Example: [[UndoSystem#^205152]]

```
ExampleObject
	- PropertyA
	- PropertyB // object
		- PropertyB1
		- PropertyB2
		- PropertyB3
	- PropertyC // object
		- PropertyC1 // array
			- PropertyC1_0
			- PropertyC1_1
			- PropertyC1_2
			- PropertyC1_3
		- PropertyC2
```

```
// make a change to ExampleObject/PropertyB/PropertyB3

```

### Examples
[Horizon: Zero Dawn](https://youtu.be/KRJkBxKv1VM?t=1835)
``` cpp 
// get the object to edit
SpotLightResource spotlight = GetSpotLightResource();

// take a snapshot of the object's current state
DiffUtil diff;
diff.TakeSnapshot(spotlight);

// update a value
spotlight->SetConeAngle(30.f);

// indicate that we have finished the change
diff.CommitChanges("Set Cone Angle to 30 degrees");
```

^205152

## Links
- [Creating a Tools Pipeline for Horizon: Zero Dawn](https://youtu.be/KRJkBxKv1VM?t=1542)
- [# Writing Tools Faster: Design Decisions to Accelerate Tool Development](https://youtu.be/W_NOGFpTv3U?t=1136)
- https://ourmachinery.com/post/the-story-behind-the-truth-designing-a-data-model/
- https://github.com/nlohmann/json
- https://github.com/eliasdaler/MetaStuff
- https://eliasdaler.github.io/meta-stuff/