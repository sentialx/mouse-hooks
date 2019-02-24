const mouseEvents = require("./dist/index");

mouseEvents.default.on("mouse-up", data => {
  console.log(data);
});
