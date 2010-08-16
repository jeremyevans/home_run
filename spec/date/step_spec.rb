require File.expand_path('../../spec_helper', __FILE__)

describe "Date#step" do
  
  it "should be able to step forward in time" do
    ds    = Date.civil(2008, 10, 11)
    de    = Date.civil(2008,  9, 29)
    count = 0
    de.step(ds) do |d|
      d.should <= ds
      d.should >= de
      count += 1
    end.should == de
    count.should == 13

    count = 0
    de.step(ds, 5) do |d|
      d.should <= ds
      d.should >= de
      count += 1
    end.should == de
    count.should == 3

    count = 0
    ds.step(de) do |d|; count += 1; end.should == ds
    count.should == 0

  end
  
  it "should be able to step backward in time" do
    ds    = Date.civil(2000, 4, 14)
    de    = Date.civil(2000, 3, 29)
    count = 0
    ds.step(de, -1) do |d|
      d.should <= ds
      d.should >= de
      count += 1
    end.should == ds
    count.should == 17

    count = 0
    ds.step(de, -5) do |d|
      d.should <= ds
      d.should >= de
      count += 1
    end.should == ds
    count.should == 4

    count = 0
    de.step(ds, -1) do |d|; count += 1; end.should == de
    count.should == 0
    
  end
  
  it "should yield once if the dates are the same, regardless of step" do
    ds = Date.civil(2008, 10, 11)
    count = 0
    ds.step(ds, 1) do |d|
      d.should == ds
      count += 1
    end.should == ds
    count.should == 1

    count = 0
    ds.step(ds, 0) do |d|
      d.should == ds
      count += 1
    end.should == ds
    count.should == 1

    count = 0
    ds.step(ds, -1) do |d|
      d.should == ds
      count += 1
    end.should == ds
    count.should == 1
  end
  
  it "should not yield if the target date is greater than the receiver, and step is not positive" do
    ds = Date.civil(2008, 10, 11)
    count = 0
    ds.step(ds.next, 0) do |d|
      count += 1
    end.should == ds
    count.should == 0

    count = 0
    ds.step(ds.next, -1) do |d|
      count += 1
    end.should == ds
    count.should == 0
  end
  
  it "should not yield if the target date is less than the receiver, and step is not negative" do
    ds = Date.civil(2008, 10, 11)
    count = 0
    ds.next.step(ds, 0) do |d|
      count += 1
    end.should == ds.next
    count.should == 0

    count = 0
    ds.next.step(ds, 1) do |d|
      count += 1
    end.should == ds.next
    count.should == 0
  end
  
end
