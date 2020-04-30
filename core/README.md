# Core of the Serio application

Consists of several modules, built on top of each
other, encapsulated behind the facade-class "org.serio.core.Core".

Each package inside the "org.serio.core" represents such
module. Each module has is its own facade-class, intended to be used
to access the module's functionality.

Do not use the modules directly. Use only the Core class to reduce the coupling.