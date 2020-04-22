package org.serio.core.showcrawlerlogstorage;

import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.*;

public class CrawlLogEntryTest {
    private String value, input, output;

    @Before
    public void setUp() throws Exception {
        value = "A crawl log entry body";
        input = "Input arguments";
        output = "Output arguments";
    }

    @Test
    public void shouldHaveSpecifiedValue() {
        // when
        CrawlLogEntry entry = new CrawlLogEntry(value);
        // then
        assertEquals(value, entry.getValue());
    }

    @Test(expected = IllegalArgumentException.class)
    public void shouldNotHaveValueNotSpecified() {
        // when
        new CrawlLogEntry(null);
    }

    @Test
    public void shouldHaveSpecifiedInputAndOutputInformation() {
        // when
        CrawlLogEntry entry = new CrawlLogEntry(value, input, output);
        // then
        assertEquals(input, entry.getInputInformation().orElse(null));
        assertEquals(output, entry.getOutputInformation().orElse(null));
    }

    @Test
    public void shouldNotHaveInputAndOutputInformationSpecified() {
        // when
        CrawlLogEntry entry = new CrawlLogEntry(value);
        // then
        assertFalse(entry.getInputInformation().isPresent());
        assertFalse(entry.getOutputInformation().isPresent());
    }

    @Test
    public void shouldHaveDetailsIfItHasInputAndOutputInformation() {
        // when
        CrawlLogEntry entry =  new CrawlLogEntry(value, input, output);
        // then
        assertTrue(entry.hasDetails());
    }

    @Test
    public void shouldNotHaveDetailsIfItDoesNotHaveInputAndOutputInformation() {
        // when
        CrawlLogEntry entry = new CrawlLogEntry(value);
        // then
        assertFalse(entry.hasDetails());
    }

    @Test
    public void shouldBeEqualAndHaveSameHash() {
        // when
        CrawlLogEntry entry1 = new CrawlLogEntry(value, input, output);
        CrawlLogEntry entry2 = new CrawlLogEntry(value, input, output);
        // then
        assertEquals(entry1, entry2);
        assertEquals(entry1.hashCode(), entry2.hashCode());
    }

    @Test
    public void shouldNotBeEqual() {
        // when
        CrawlLogEntry entry1 = new CrawlLogEntry(value, input, output);
        CrawlLogEntry entry2 = new CrawlLogEntry(value);
        // then
        assertNotEquals(entry1, entry2);
        assertNotEquals(entry1.hashCode(), entry2.hashCode());
    }
}
