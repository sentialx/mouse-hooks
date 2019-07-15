import { EventEmitter } from "events";
import { fork, ChildProcess } from "child_process";
import { join } from "path";
import { platform } from "os";

let registeredEvents: string[] = [];

let mouseProcess: ChildProcess;

class MouseEvents extends EventEmitter {
  constructor() {
    super();

    if (platform() !== "win32") return;

    this.on("newListener", event => {
      if (registeredEvents.indexOf(event) !== -1) return;

      if (
        (event === "mouse-up" ||
          event === "mouse-down" ||
          event === "mouse-move") &&
        !mouseProcess
      ) {
        mouseProcess = fork(join(__dirname, "../mouse.js"));

        mouseProcess.on("message", msg => {
          this.emit(msg.event, { x: msg.x, y: msg.y, button: msg.button });
        });
      } else {
        return;
      }

      registeredEvents.push(event);
    });

    this.on("removeListener", event => {
      if (this.listenerCount(event) > 0) return;

      if (
        (event === "mouse-up" ||
          event === "mouse-down" ||
          event === "mouse-move") &&
        mouseProcess
      ) {
        mouseProcess.kill();
      }

      registeredEvents = registeredEvents.filter(x => x !== event);
    });
  }
}

const mouseEvents = new MouseEvents();

export default mouseEvents;
