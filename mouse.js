const addon = require("bindings")("addon");

addon.createMouseHook((event, x, y, which) => {
  process.send({ event, x, y, button: which });
});
