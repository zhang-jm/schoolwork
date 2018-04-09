/*
* Name: Jamie Zhang
* Login: cs11fmd
* Date: November 11, 2014
* File: AnimalCard.java
* Sources of Help: PA6 instructions
*/

interface AnimalCard extends Speakable, Highlightable, Hideable
{
  @Override
  public abstract boolean equals(Object o);
}
