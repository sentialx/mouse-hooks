const mouseEvents = require("./dist/index");

mouseEvents.default.on("mouse-up", data => {
  console.log(data);
});

mouseEvents.default.on("mouse-move", data => {
  console.log(data);
});
