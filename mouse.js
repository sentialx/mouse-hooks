const addon = require("bindings")("windows-mouse-hooks");

addon.createMouseHook(event => {
  process.send(event);
});
