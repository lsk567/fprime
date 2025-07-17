module Svc {

  # FIXME: File an issue that proposes to make this an active component.
  @ A Linux interval timer
  passive component LinuxTimer {

    @ implement tick interface
    import Drv.Tick

  }

}
