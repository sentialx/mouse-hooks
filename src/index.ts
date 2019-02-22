import { EventEmitter } from "events";
import { fork, ChildProcess } from "child_process";
import { join } from 'path';

let registeredEvents: string[] = [];

let mouseProcess: ChildProcess;

class MouseEvents extends EventEmitter {
  constructor() {
    super();

    this.on("newListener", event => {
      if (registeredEvents.indexOf(event) !== -1) return;

      if (event === "mouse-up" && !mouseProcess) {
        mouseProcess = fork(join(__dirname, "../mouse.js"));

        mouseProcess.on("message", msg => {
          if (msg === "mouse-up") {
            this.emit("mouse-up");
          }
        });
      } else {
        return;
      }

      registeredEvents.push(event);
    });

    this.on("removeListener", event => {
      if (this.listenerCount(event) > 0) return;

      if (event === "mouse-up" && mouseProcess) {
        mouseProcess.kill();
      }

      registeredEvents = registeredEvents.filter(x => x !== event);
    });
  }
}

const mouseEvents = new MouseEvents();

export default mouseEvents;
