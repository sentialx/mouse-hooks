# mouse-hooks

Global mouse events listener for Node.js

# Installation

```bash
$ npm install mouse-hooks
```

# Quick example

```ts
import mouseHooks from "mouse-hooks";

mouseHooks.on("mouse-up", data => {
  console.log(data.x, data.y, data.button);
});
```
