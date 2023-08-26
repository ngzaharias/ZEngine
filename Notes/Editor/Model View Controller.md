# Model View Controller

## Model
- Holds the data
- Can only be modified by the controller

## Views
- Holds the most recent copy of the data
- Refreshes its copy when it receives a signal that the model has changed
- Signals to the controller when a change has been made
- Changes to data are done via a Transaction

## Controller
- Emits signals when a View modifies data so that the Model can apply the change and so other Views can refresh their data.

### Links
- [Creating a Tools Pipeline for Horizon: Zero Dawn](https://www.youtube.com/watch?v=KRJkBxKv1VM&t=1542s)
- [Model View Controller Pattern](https://www.bogotobogo.com/DesignPatterns/mvc_model_view_controller_pattern.php)