const addon = require("bindings")("windows-mouse-hooks");

addon.createMouseHook((event, x, y, which) => {
  process.send({ event, x, y, button: which });
});
